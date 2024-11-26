import {rle_compress, rle_decompress} from '/static/compresso/algorithms/text/rle.js'
import {lzw_compress, lzw_decompress} from '/static/compresso/algorithms/text/lzw.js';
import {huffman_compress, huffman_decompress} from '/static/compresso/algorithms/text/huffman.js';

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
        },
        "Huffman":{
            "Compression" : huffman_compress,
            "Decompression" : huffman_decompress
        }
    }

    var algorithm = document.querySelector("#algorithm").value;
    var operation = document.querySelector("#operation").value;
    var plain_text = document.querySelector('#plaintext').value;
    var parsed_text = algorithm_map[algorithm][operation](plain_text)

    document.querySelector('#parsedtext').value = parsed_text;
}