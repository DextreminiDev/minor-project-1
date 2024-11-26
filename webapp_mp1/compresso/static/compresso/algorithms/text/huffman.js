export function huffman_compress(input) {
    if (!input) {
        return "Invalid input";
    }

    // Frequency table
    const freq = {};
    for (const char of input) {
        freq[char] = (freq[char] || 0) + 1;
    }

    // Create nodes
    const nodes = Object.entries(freq).map(([char, frequency]) => ({ char, frequency, left: null, right: null }));
    nodes.sort((a, b) => a.frequency - b.frequency);

    // Build the Huffman tree
    while (nodes.length > 1) {
        const left = nodes.shift();
        const right = nodes.shift();
        const internalNode = {
            char: null,
            frequency: left.frequency + right.frequency,
            left,
            right,
        };
        nodes.push(internalNode);
        nodes.sort((a, b) => a.frequency - b.frequency);
    }

    // Generate codes
    const root = nodes[0];
    const codes = {};
    const generateCodes = (node, code = "") => {
        if (!node) return;
        if (node.char !== null) {
            codes[node.char] = code;
        }
        generateCodes(node.left, code + "0");
        generateCodes(node.right, code + "1");
    };
    generateCodes(root);

    // Compress the input
    const compressedData = input.split("").map(char => codes[char]).join("");

    // Return compressed result without labels
    return JSON.stringify({
        data: compressedData,
        codes,
    }, null, 4); // Pretty-print with 4 spaces
}

export function huffman_decompress(input) {
    try {
        const { data, codes } = JSON.parse(input);

        if (!data || !codes) {
            return "Invalid input format";
        }

        // Reverse the codes for decoding
        const reversedCodes = Object.entries(codes).reduce((acc, [char, code]) => {
            acc[code] = char;
            return acc;
        }, {});

        // Decompress the data
        let currentCode = "";
        let decompressedData = "";

        for (const bit of data) {
            currentCode += bit;
            if (reversedCodes[currentCode]) {
                decompressedData += reversedCodes[currentCode];
                currentCode = "";
            }
        }

        // Return decompressed result without labels
        return decompressedData;
    } catch (error) {
        return "Invalid JSON input";
    }
}
