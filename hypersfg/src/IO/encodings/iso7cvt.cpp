#include "8bit.h"
#include "iso7cvt.h"

using namespace std;


static const wchar_t iso2ucs[] = {
	0x00A0, 0x2018, 0x2019, 0x00A3, 0x20AC, 0x20AF, 0x00A6, 0x00A7,
	0x00A8, 0x00A9, 0x037A, 0x00AB, 0x00AC, 0x00AD, 0x0000, 0x2015,
	0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x0384, 0x0385, 0x0386, 0x00B7,
	0x0388, 0x0389, 0x038A, 0x00BB, 0x038C, 0x00BD, 0x038E, 0x038F,
	0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397,
	0x0398, 0x0399, 0x039A, 0x039B, 0x039C, 0x039D, 0x039E, 0x039F,
	0x03A0, 0x03A1, 0x0000, 0x03A3, 0x03A4, 0x03A5, 0x03A6, 0x03A7,
	0x03A8, 0x03A9, 0x03AA, 0x03AB, 0x03AC, 0x03AD, 0x03AE, 0x03AF,
	0x03B0, 0x03B1, 0x03B2, 0x03B3, 0x03B4, 0x03B5, 0x03B6, 0x03B7,
	0x03B8, 0x03B9, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BE, 0x03BF,
	0x03C0, 0x03C1, 0x03C2, 0x03C3, 0x03C4, 0x03C5, 0x03C6, 0x03C7,
	0x03C8, 0x03C9, 0x03CA, 0x03CB, 0x03CC, 0x03CD, 0x03CE, 0x0000
};

static const struct cw ucs2iso[] = {
	{0xA0, 0x00A0}, {0xA3, 0x00A3}, {0xA6, 0x00A6}, {0xA7, 0x00A7},
	{0xA8, 0x00A8}, {0xA9, 0x00A9}, {0xAB, 0x00AB}, {0xAC, 0x00AC},
	{0xAD, 0x00AD}, {0xB0, 0x00B0}, {0xB1, 0x00B1}, {0xB2, 0x00B2},
	{0xB3, 0x00B3}, {0xB7, 0x00B7}, {0xBB, 0x00BB}, {0xBD, 0x00BD},
	{0xAA, 0x037A}, {0xB4, 0x0384}, {0xB5, 0x0385}, {0xB6, 0x0386},
	{0xB8, 0x0388}, {0xB9, 0x0389}, {0xBA, 0x038A}, {0xBC, 0x038C},
	{0xBE, 0x038E}, {0xBF, 0x038F}, {0xC0, 0x0390}, {0xC1, 0x0391},
	{0xC2, 0x0392}, {0xC3, 0x0393}, {0xC4, 0x0394}, {0xC5, 0x0395},
	{0xC6, 0x0396}, {0xC7, 0x0397}, {0xC8, 0x0398}, {0xC9, 0x0399},
	{0xCA, 0x039A}, {0xCB, 0x039B}, {0xCC, 0x039C}, {0xCD, 0x039D},
	{0xCE, 0x039E}, {0xCF, 0x039F}, {0xD0, 0x03A0}, {0xD1, 0x03A1},
	{0xD3, 0x03A3}, {0xD4, 0x03A4}, {0xD5, 0x03A5}, {0xD6, 0x03A6},
	{0xD7, 0x03A7}, {0xD8, 0x03A8}, {0xD9, 0x03A9}, {0xDA, 0x03AA},
	{0xDB, 0x03AB}, {0xDC, 0x03AC}, {0xDD, 0x03AD}, {0xDE, 0x03AE},
	{0xDF, 0x03AF}, {0xE0, 0x03B0}, {0xE1, 0x03B1}, {0xE2, 0x03B2},
	{0xE3, 0x03B3}, {0xE4, 0x03B4}, {0xE5, 0x03B5}, {0xE6, 0x03B6},
	{0xE7, 0x03B7}, {0xE8, 0x03B8}, {0xE9, 0x03B9}, {0xEA, 0x03BA},
	{0xEB, 0x03BB}, {0xEC, 0x03BC}, {0xED, 0x03BD}, {0xEE, 0x03BE},
	{0xEF, 0x03BF}, {0xF0, 0x03C0}, {0xF1, 0x03C1}, {0xF2, 0x03C2},
	{0xF3, 0x03C3}, {0xF4, 0x03C4}, {0xF5, 0x03C5}, {0xF6, 0x03C6},
	{0xF7, 0x03C7}, {0xF8, 0x03C8}, {0xF9, 0x03C9}, {0xFA, 0x03CA},
	{0xFB, 0x03CB}, {0xFC, 0x03CC}, {0xFD, 0x03CD}, {0xFE, 0x03CE},
	{0xAF, 0x2015}, {0xA1, 0x2018}, {0xA2, 0x2019}, {0xA4, 0x20AC},
	{0xA5, 0x20AF}
};


codecvt_base::result iso7cvt::do_in(mbstate_t&, const char* from,
  const char* from_end, const char*& from_next, wchar_t* to,
  wchar_t* to_limit, wchar_t*& to_next) const
{
	return ::do_in(from, from_end, from_next, to, to_limit, to_next, 0xA0,
	  iso2ucs) ? codecvt_base::error : codecvt_base::ok;
}

codecvt_base::result iso7cvt::do_out(mbstate_t&, const wchar_t* from,
  const wchar_t* from_end, const wchar_t*& from_next, char* to,
  char* to_limit, char*& to_next) const
{
	return ::do_out(from, from_end, from_next, to, to_limit, to_next, 0xA0,
	  ucs2iso, sizeof(ucs2iso)) ? codecvt_base::error : codecvt_base::ok;
}

codecvt_base::result iso7cvt::do_unshift(mbstate_t&, char* to, char*,
  char*& to_next) const
{
	to_next = to;

	return codecvt_base::noconv;
}
