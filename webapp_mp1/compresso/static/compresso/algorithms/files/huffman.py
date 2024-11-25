import os
import heapq
from collections import defaultdict

# Helper class to represent the nodes in the Huffman Tree
class Node:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq


def build_huffman_tree(data):
    # Calculate frequency of each character
    freq = defaultdict(int)
    for char in data:
        freq[char] += 1

    # Create a priority queue (min-heap)
    priority_queue = [Node(char, freq[char]) for char in freq]
    heapq.heapify(priority_queue)

    while len(priority_queue) > 1:
        # Pop two nodes with the lowest frequencies
        left = heapq.heappop(priority_queue)
        right = heapq.heappop(priority_queue)

        # Create a new internal node with a combined frequency
        internal_node = Node(None, left.freq + right.freq)
        internal_node.left = left
        internal_node.right = right

        # Push the internal node back into the priority queue
        heapq.heappush(priority_queue, internal_node)

    return priority_queue[0]  # The root of the Huffman tree


def generate_huffman_codes(root, code="", codes=None):
    if codes is None:
        codes = {}

    if root:
        if root.char is not None:  # It's a leaf node
            codes[root.char] = code
        generate_huffman_codes(root.left, code + "0", codes)
        generate_huffman_codes(root.right, code + "1", codes)

    return codes


def huffman(input_file, mode):
    base_name = os.path.splitext(os.path.basename(input_file))[0]
    parent_dir = os.path.dirname(input_file)

    if mode == "compress":
        with open(input_file, "r") as file:
            data = file.read()

        root = build_huffman_tree(data)
        huffman_codes = generate_huffman_codes(root)

        compressed_data = ''.join(huffman_codes[char] for char in data)

        output_file = os.path.join(parent_dir, f"{base_name}.huff")
        with open(output_file, "w") as file:
            file.write(f"{len(huffman_codes)}\n")
            for char, code in huffman_codes.items():
                file.write(f"{repr(char)}:{code}\n")
            file.write(compressed_data)

        print(f"Compression successful! File saved at: {output_file}")
        return output_file

    elif mode == "decompress":
        with open(input_file, "r") as file:
            try:
                # Read the number of Huffman codes
                num_codes = int(file.readline().strip())
            except ValueError as e:
                print("Error reading the number of codes:", e)
                return None

            huffman_codes = {}
            # Parse the Huffman codes from the file
            for _ in range(num_codes):
                line = file.readline().strip()
                try:
                    char, code = line.split(":")
                    char = eval(char)  # Convert the string representation back to the original character
                    huffman_codes[code] = char
                except ValueError:
                    print(f"Error parsing line: {line}")
                    return None

            # Read the compressed binary data
            compressed_data = file.read().strip()

        print("DEBUG: Huffman codes:", huffman_codes)
        print("DEBUG: Compressed data:", compressed_data)

        # Decode the compressed data
        current_code = ""
        decompressed_data = []  # Using a list here
        for bit in compressed_data:
            current_code += bit
            if current_code in huffman_codes:
                decompressed_data.append(huffman_codes[current_code])
                current_code = ""  # Reset the code for the next character

        # Join the decompressed characters
        decompressed_data = ''.join(decompressed_data)

        # Save the decompressed data
        output_file = os.path.join(parent_dir, f"{base_name}_decompressed.txt")
        with open(output_file, "w") as file:
            file.write(decompressed_data)

        print(f"Decompression successful! File saved at: {output_file}")
        return output_file

    else:
        print("Invalid mode! Use 'compress' or 'decompress'.")
        return None
