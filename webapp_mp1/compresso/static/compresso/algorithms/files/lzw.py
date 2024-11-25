import os

def lzw(input_file, mode):
    base_name = os.path.splitext(os.path.basename(input_file))[0]
    parent_dir = os.path.dirname(input_file)

    with open(input_file, "r") as file:
        data = file.read()

    if mode == "compress":
        dictionary = {chr(i): i for i in range(256)}
        next_code = 256
        compressed = []
        current = ""

        for char in data:
            new_seq = current + char
            if new_seq in dictionary:
                current = new_seq
            else:
                compressed.append(dictionary[current])
                dictionary[new_seq] = next_code
                next_code += 1
                current = char

        if current:
            compressed.append(dictionary[current])

        output_file = os.path.join(parent_dir, f"{base_name}.lzw")

        with open(output_file, "w") as file:
            file.write(" ".join(map(str, compressed)))

        return output_file

    elif mode == "decompress":
        dictionary = {i: chr(i) for i in range(256)}
        next_code = 256
        compressed = list(map(int, data.split()))
        decompressed = ""
        current = chr(compressed.pop(0))
        decompressed += current

        for code in compressed:
            if code in dictionary:
                entry = dictionary[code]
            elif code == next_code:
                entry = current + current[0]
            else:
                raise ValueError("Invalid LZW code encountered during decompression")

            decompressed += entry
            dictionary[next_code] = current + entry[0]
            next_code += 1
            current = entry

        output_file = os.path.join(parent_dir, f"{base_name}_decompressed.txt")

        with open(output_file, "w") as file:
            file.write(decompressed)

        return output_file
