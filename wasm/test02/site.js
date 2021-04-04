// Delay loading any function until the html dom has loaded. All functions are
// defined in this top level function to ensure private scope.

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

jQuery(document).ready(function () {

  // Installs error handling.
  jQuery.ajaxSetup({
  error: function(resp, e) {
    if (resp.status == 0){
      alert('You are offline!!\n Please Check Your Network.');
      } else if (resp.status == 404){
        alert('Requested URL not found.');
      } else if (resp.status == 500){
        alert('Internel Server Error:\n\t' + resp.responseText);
      } else if (e == 'parsererror') {
        alert('Error.\nParsing JSON Request failed.');
      } else if (e == 'timeout') {
        alert('Request timeout.');
      } else {
        alert('Unknown Error.\n' + resp.responseText);
      }
    }
  });  // error:function()


  var generate_btn = jQuery('#generate_btn');

  var sample_1_btn = jQuery('#sample_1_btn');
  var sample_2_btn = jQuery('#sample_2_btn');
  var sample_3_btn = jQuery('#sample_3_btn');
  var sample_4_btn = jQuery('#sample_4_btn');
  var sample_5_btn = jQuery('#sample_5_btn');

  var svg_div = jQuery('#graph_svg_div');
  var graph_data_textarea = jQuery('#graph_data');

  function InsertGraphText(text) {
    graph_data_textarea.val(text);
  }


  function UpdateGraph() {
    svg_div.html("");
    var data = graph_data_textarea.val();


    // Generate the Visualization of the Graph into "svg".

      (async () => {
        // Build the WebAssembly instance.
        // initial value is 64Kb * max memory
        const memory = new WebAssembly.Memory({ initial: 10 });
        const response = await fetch('graph2svg.wasm');
        const bytes = await response.arrayBuffer();

        const { instance } = await WebAssembly.instantiate(bytes, {
          env: { memory }
        });

        // the svg image data to inline in the html
        var svg ="";

        // number of chars in resulting svg
        var svgnchar = 0;
        var svgnchar2 = 0;

        // Text to copy.
        const text =  graph_data_textarea.val();

        // Configure shared memory.
        const view = new Uint8Array(memory.buffer);
        const pInput = instance.exports.__heap_base;
        const poutput = instance.exports.__heap_base + 1024;

        // limit the amount of max. input text
        if(text.length < 1000){

          // copy text to pInput
          encode(view, pInput, text);

          // run our own graph layout in sfg.c return number of chars in svg
          svgnchar = instance.exports.graph2svg(pInput,poutput);
          svgnchar2 = instance.exports.svgdata(pInput,svgnchar);

        } else {
          // set to message too-much-input-text
          svgnchar = -1;
        }

       // console.log(svgnchar);
       // console.log(svgnchar2);

        if (svgnchar > 0) {
           // create copy of svg data
           // old:  svg = decode(view, poutput);
           svg = decode(view, pInput);
        } else {
           if (svgnchar < 0) {
             svg = "too much input text in textarea maximum 1000 chars";
           } else {
             svg = "no svg data";
           }
        }

        // console.log(svg);

       // set svg data as html data
       svg_div.html("<hr>"+svg);

      })();


      // Encode string into memory starting at address base.
      const encode = (memory, base, string) => {
        for (let i = 0; i < string.length; i++) {
          memory[base + i] = string.charCodeAt(i);
        }
        memory[base + string.length] = 0;
      };

      // Decode a string from memory starting at address base.
      const decode = (memory, base) => {
        let cursor = base;
        let result = '';

        while (memory[cursor] !== 0) {
          result += String.fromCharCode(memory[cursor++]);
        }
        //console.log(result);
        return result;
      };


  }

  // Startup function: call UpdateGraph
  jQuery(function() {
    // The buttons are disabled, enable them now that this script has loaded.
    generate_btn.removeAttr("disabled")
                .text("Generate Graph layout");

    sample_1_btn.removeAttr("disabled");
    sample_2_btn.removeAttr("disabled");
    sample_3_btn.removeAttr("disabled");
    sample_4_btn.removeAttr("disabled");
    sample_5_btn.removeAttr("disabled");

  });

  // Bind actions to form buttons.
  generate_btn.click(UpdateGraph);

  sample_1_btn.click(function(){
    InsertGraphText(jQuery("#sample1_text").html().trim());
  });

  sample_2_btn.click(function(){
    InsertGraphText(jQuery("#sample2_text").html().trim());
  });

  sample_3_btn.click(function(){
    InsertGraphText(jQuery("#sample3_text").html().trim());
  });

  sample_4_btn.click(function(){
    InsertGraphText(jQuery("#sample4_text").html().trim());
  });

  sample_5_btn.click(function(){
    InsertGraphText(jQuery("#sample5_text").html().trim());
  });

});
