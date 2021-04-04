
/*
 *  Copyright t lefering
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  These are the four essential freedoms with GNU GPL software:
 *  1: freedom to run the program, for any purpose
 *  2: freedom to study how the program works, and change it to make it do what you wish
 *  3: freedom to redistribute copies to help your Free Software friends
 *  4: freedom to distribute copies of your modified versions to your Free Software friends
 *   ,           ,
 *  /             \
 * ((__-^^-,-^^-__))
 * `-_---'  `---_-'
 *  `--|o`   'o|--'
 *      \  `  /
 *       ): :(
 *       :o_o:
 *        "-"
 */

// This file is a wrapper around malloc.c, which is the upstream source file.
// It sets configuration flags and controls which symbols are exported.
#include <stdio.h>
#include <stddef.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

// Define configuration macros for dlmalloc.
#define LACKS_SYS_TYPES_H 1

// WebAssembly doesn't have mmap-style memory allocation.
#define HAVE_MMAP 0

// WebAssembly doesn't support shrinking linear memory.
#define MORECORE_CANNOT_TRIM 1

// Disable sanity checks to reduce code size.
#define ABORT __builtin_unreachable()

// If threads are enabled, enable support for threads.
#ifdef _REENTRANT
#define USE_LOCKS 1
#endif

// Make malloc deterministic.
#define LACKS_TIME_H 1

// Disable malloc statistics generation to reduce code size.
#define NO_MALLINFO 1
#define NO_MALLOC_STATS 1

// Align malloc regions to 16, to avoid unaligned SIMD accesses.
#define MALLOC_ALIGNMENT 16

// Declare errno values used by dlmalloc. We define them like this to avoid
// putting specific errno values in the ABI.
//extern const int __ENOMEM;
//#define ENOMEM __ENOMEM
//extern const int __EINVAL;
//#define EINVAL __EINVAL

// Define USE_DL_PREFIX so that we leave dlmalloc's names prefixed with 'dl'.
// We define them as "static", and we wrap them with public names below. This
// serves two purposes:
//
// One is to make it easy to control which symbols are exported; dlmalloc
// defines several non-standard functions and we wish to explicitly control
// which functions are part of our public-facing interface.
//
// The other is to protect against compilers optimizing based on the assumption
// that they know what functions with names like "malloc" do. Code in the
// implementation will call functions like "dlmalloc" and assume it can use
// the resulting pointers to access the metadata outside of the nominally
// allocated objects. However, if the function were named "malloc", compilers
// might see code like that and assume it has undefined behavior and can be
// optimized away. By using "dlmalloc" in the implementation, we don't need
// -fno-builtin to avoid this problem.
#define USE_DL_PREFIX 1
#define DLMALLOC_EXPORT static inline

// This isn't declared with DLMALLOC_EXPORT so make it static explicitly.
static size_t dlmalloc_usable_size(void *);

void abort(void)
{
	// wasm doesn't support signals, so just trap to halt the program.
	__builtin_trap();
}

int errno = 0;

/*
 * The page size in WebAssembly is fixed at 64 KiB. If this ever changes,
 * it's expected that applications will need to opt in, so we can change
 * this.
 */
#define PAGESIZE (0x10000)

#define sysconf(name) PAGESIZE
#define _SC_PAGESIZE

#define BLKSIZ PAGESIZE

static void *allocateMemory(unsigned bytes_required);
static void freeMemory(void *startOfMemoryToFree);

typedef long align;

// header of an allocation block
typedef union header {
	struct {
		union header *next;	// Pointer to circular successor
		unsigned int size;	// Size of the block
	} value;
	long align;		// Forces block alignment
} header_t;

static header_t *free_list = (header_t *) NULL;
static unsigned int initial_offset = 8;	// preserve address 0 for null and pad by 4 bytes.
static int is_initialised = 0;

static header_t *getMoreMemory(unsigned bytes_required)
{
	// We need to add the header to the bytes required.
	bytes_required += sizeof(header_t);

	// The memory gets delivered in blocks. Ensure we get enough.
	unsigned int blocks = bytes_required / BLKSIZ;
	if (blocks * BLKSIZ < bytes_required)
		blocks += 1;
	unsigned int start_of_new_memory = __builtin_wasm_memory_size(0) * BLKSIZ;

	if (__builtin_wasm_memory_grow(0, blocks) == -1)
		return NULL;

	long end_of_new_memory = __builtin_wasm_memory_size(0) * BLKSIZ;

	// Create the block to insert.
	header_t *block_to_insert = (header_t *) start_of_new_memory;
	block_to_insert->value.size = end_of_new_memory - start_of_new_memory - sizeof(header_t);
	block_to_insert->value.next = NULL;

	// add to the free list
	freeMemory((void *)(block_to_insert + 1));

	return free_list;
}

static void ensureInitialised()
{
	if (is_initialised == 0) {
		is_initialised = 1;

		// initialise the memory allocator.
		unsigned int bytes_length = __builtin_wasm_memory_size(0) * BLKSIZ;

		// Start at 1 to save 0 for NULL.
		header_t *unallocated = (header_t *) initial_offset;
		unallocated->value.size = bytes_length - (sizeof(header_t) + initial_offset);
		unallocated->value.next = NULL;

		free_list = unallocated;
	}
}

static __attribute__((used))
void *allocateMemory(unsigned bytes_required)
{
	ensureInitialised();

	// Pad to 8 bytes until I find a better solution.
	bytes_required += (8 - bytes_required % 8) % 8;
	unsigned int bytes_required_plus_header = bytes_required + sizeof(header_t);

	if (free_list == NULL) {
		free_list = getMoreMemory(bytes_required_plus_header);
		if (free_list == NULL)
			return NULL;
	}

	header_t *current = free_list;
	header_t *previous = current;
	while (current != NULL) {
		if (current->value.size == bytes_required) {
			// exact match
			if (current == free_list) {
				// allocate all of the free list
				free_list = NULL;
				current->value.next = NULL;
				return current + 1;
			} else {
				// remove the block
				previous->value.next = current->value.next;
				current->value.next = NULL;
				return current + 1;
			}
		} else if (current->value.size > bytes_required) {
			// split the bigger block

			// create the unallocated portion
			header_t *unallocated = (header_t *) ((char *)current + bytes_required_plus_header);
			unallocated->value.size = current->value.size - bytes_required_plus_header;
			unallocated->value.next = current->value.next;

			if (current == free_list) {
				// We are at the start of the list so make the free listthe unallocated block.
				free_list = unallocated;
			} else {
				// We past the start of the list so remove the current block.
				previous->value.next = unallocated;
			}

			// prepare the allocated portion.
			current->value.size = bytes_required;
			current->value.next = NULL;

			return current + 1;
		}

		previous = current;
		current = current->value.next;
	}

	// No block was big enough. Grow the memory and try again
	if (getMoreMemory(bytes_required) == NULL)
		return NULL;

	return allocateMemory(bytes_required_plus_header);
}

static __attribute__((used))
void freeMemory(void *ptr)
{
	ensureInitialised();

	if (ptr == NULL)
		return;

	header_t *unallocated = ((header_t *) ptr) - 1;

	if (free_list == NULL) {
		// If the free list is null the unallocated block becomes the free list.
		free_list = unallocated;
		return;
	}
	// Find the place in the free list where the unallocated block should be inserted.
	header_t *current = free_list;
	header_t *previous = current;
	while (current != NULL) {
		if (unallocated > previous && unallocated < current)
			break;	// The unallocated block is between the previous and current.
		else if (current == previous && unallocated < current) {
			// There is only one block in the list and it is after the unallocated.
			previous = NULL;
			break;
		}
		// Step forward.
		previous = current;
		current = current->value.next;
	}

	// Attach the unallocated block to the current block.
	if (current != NULL) {
		// Are the blocks adjacent?
		if (current == (header_t *) ((char *)(unallocated + 1) + unallocated->value.size)) {
			// Merge the unallocated with the current block.
			unallocated->value.size += current->value.size + sizeof(header_t);
			unallocated->value.next = current->value.next;
		} else {
			// Chain the unallocated block to the current.
			unallocated->value.next = current;
		}
	}

	if (previous == NULL) {
		// The unallocated block now starts the free list.
		free_list = unallocated;
	} else {
		// Are the blocks adjacent?
		if (unallocated == (header_t *) ((char *)(previous + 1) + previous->value.size)) {
			// Merge the previous block with the unallocated.
			previous->value.size += unallocated->value.size + sizeof(header_t);
			previous->value.next = unallocated->value.next;
		} else {
			// Chain the previous block to the unallocated.
			previous->value.next = unallocated;
		}
	}
}

static __attribute__((used))
double reportFreeMemory()
{
	ensureInitialised();

	if (free_list == NULL)
		return 0;

	header_t *current = free_list;
	unsigned int total = 0;
	while (current != NULL) {
		total += current->value.size;
		current = current->value.next;
	}

	return (double)total;
}

// Bare-bones implementation of sbrk.
void *sbrk(size_t increment)
{
	// sbrk(0) returns the current memory size.
	if (increment == 0) {
		// The wasm spec doesn't guarantee that memory.grow of 0 always succeeds.
		return (void *)(__builtin_wasm_memory_size(0) * PAGESIZE);
	}
	return (allocateMemory(increment));
}

// Include the upstream dlmalloc's malloc.c.
#define LACKS_SYS_TYPES_H 1

#include "malloc.c"

// Export the public names.

void *malloc(size_t size)
{
	return dlmalloc(size);
}

void free(void *ptr)
{
	dlfree(ptr);
}

void *calloc(size_t nmemb, size_t size)
{
	return dlcalloc(nmemb, size);
}

void *realloc(void *ptr, size_t size)
{
	return dlrealloc(ptr, size);
}

#if 0
int posix_memalign(void **memptr, size_t alignment, size_t size)
{
	return dlposix_memalign(memptr, alignment, size);
}

void *aligned_alloc(size_t alignment, size_t bytes)
{
	return dlmemalign(alignment, bytes);
}

size_t malloc_usable_size(void *ptr)
{
	return dlmalloc_usable_size(ptr);
}

// Define these to satisfy musl references.
void *__libc_malloc(size_t) __attribute__((alias("malloc")));
void __libc_free(void *) __attribute__((alias("free")));
void *__libc_calloc(size_t nmemb, size_t size) __attribute__((alias("calloc")));
#endif
