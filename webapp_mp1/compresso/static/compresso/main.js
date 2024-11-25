import {rle_compress, rle_decompress} from '/static/compresso/algorithms/text/rle.js'
// import {lzw_compress, lzw_decompress} from '/static/compresso/algorithms/text/lzw.js'

document.addEventListener('DOMContentLoaded', function() {
    document.querySelector('#exec').addEventListener('click', () => text_compression_decompression()); 
}); 

function text_compression_decompression(){
    var algorithm_map = {
        "RLE":{
            "Compression" : rle_compress,
            "Decompression" : rle_decompress
        },
        "LZW":{
            "Compression" : lzw_compress,
            "Decompression" : lzw_decompress
        }
    }

    var algorithm = document.querySelector("#algorithm").value;
    var operation = document.querySelector("#operation").value;
    console.log("Selected algorithm:", algorithm);
    console.log("Selected operation:", operation);
    var plain_text = document.querySelector('#plaintext').value;
    console.log(plain_text);
    var parsed_text = algorithm_map[algorithm][operation](plain_text)
    console.log(parsed_text);
    document.querySelector('#parsedtext').value = parsed_text;
}