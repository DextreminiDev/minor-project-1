export function rle_compress(input_string){
    /**
     * Compresses a string using Run-Length Encoding(RLE).
     * @param{string} input_string - The string to compress.
     * @return{string} The compressed string.
     */
    if(!input_string){
        return "";
    }

    let compressed = "";
    let count = 1;

    for(let i = 1; i < input_string.length; i++){
        if(input_string[i] === input_string[i - 1]){
            count++;
        } 
        else{
            compressed += input_string[i - 1] + count;
            count = 1;
        }
    }

    // Add the last character and its count
    compressed += input_string[input_string.length - 1] + count;

    return compressed;
}

export function rle_decompress(compressed_string){
    /**
     * Decompresses a string encoded with Run-Length Encoding(RLE).
     * @param{string} compressed_string - The compressed string to decompress.
     * @return{string} The decompressed string.
     */
    if(!compressed_string){
        return "";
    }

    let decompressed = "";
    let i = 0;

    while(i < compressed_string.length){
        const char = compressed_string[i];
        i++;
        let count = 0;

        // Read the count(can be multiple digits)
        while(i < compressed_string.length && !isNaN(compressed_string[i])){
            count = count * 10 + parseInt(compressed_string[i], 10);
            i++;
        }

        decompressed += char.repeat(count);
    }

    return decompressed;
}