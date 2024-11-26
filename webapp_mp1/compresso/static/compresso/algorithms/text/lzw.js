export function lzw_compress(input) {
    if (!input) {
        return "Invalid input";
    }

    // Initialize the dictionary with single-character entries
    const dictionary = {};
    for (let i = 0; i < 256; i++) {
        dictionary[String.fromCharCode(i)] = i;
    }

    let nextCode = 256;
    let current = "";
    const compressed = [];

    // Compress the input string
    for (const char of input) {
        const newSeq = current + char;
        if (dictionary[newSeq] !== undefined) {
            current = newSeq;
        } else {
            compressed.push(dictionary[current]);
            dictionary[newSeq] = nextCode++;
            current = char;
        }
    }

    // Add the last code
    if (current !== "") {
        compressed.push(dictionary[current]);
    }

    // Return the compressed data and the dictionary
    return JSON.stringify(
        {
            data: compressed,
            dictionary: Object.keys(dictionary).reduce((acc, key) => {
                if (dictionary[key] < nextCode) acc[key] = dictionary[key];
                return acc;
            }, {}),
        },
        null,
        4 // Pretty-print with 4 spaces
    );
}

export function lzw_decompress(input) {
    try {
        // Parse the input JSON
        const { data, dictionary } = JSON.parse(input);
        if (!data || !dictionary) {
            return "Invalid input format";
        }

        // Initialize the reverse dictionary for decompression
        const reverseDictionary = Object.entries(dictionary).reduce((acc, [key, value]) => {
            acc[value] = key;
            return acc;
        }, {});

        let nextCode = Math.max(...Object.keys(reverseDictionary).map(Number)) + 1;
        let current = String.fromCharCode(data[0]);
        let decompressed = current;

        // Decompress the data
        for (let i = 1; i < data.length; i++) {
            const code = data[i];
            let entry;

            if (reverseDictionary[code] !== undefined) {
                entry = reverseDictionary[code];
            } else if (code === nextCode) {
                entry = current + current[0];
            } else {
                throw new Error("Invalid LZW code encountered during decompression");
            }

            decompressed += entry;

            // Add new entry to the dictionary
            reverseDictionary[nextCode++] = current + entry[0];
            current = entry;
        }

        // Return the decompressed data as a plain string
        return decompressed;
    } catch (error) {
        return "Invalid JSON input";
    }
}
