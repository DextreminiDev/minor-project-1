import os

def rle(input_file, mode):
    base_name = os.path.splitext(os.path.basename(input_file))[0]
    parent_dir = os.path.dirname(input_file)  

    with open(input_file, "r") as file:
        data = file.read()

    if mode == "compress":
        compressed = ""
        i = 0

        while i < len(data):
            count = 1
            while i + 1 < len(data) and data[i] == data[i + 1]:
                count += 1
                i += 1
            compressed += data[i] + str(count)
            i += 1

        output_file = os.path.join(parent_dir, f"{base_name}.rle")
        
        with open(output_file, "w") as file:
            file.write(compressed)

        return output_file

    elif mode == "decompress":
        decompressed = ""
        i = 0

        while i < len(data):
            char = data[i]
            count = ""
            i += 1
            while i < len(data) and data[i].isdigit():
                count += data[i]
                i += 1
            decompressed += char * int(count)

        output_file = os.path.join(parent_dir, f"{base_name}_decompressed.txt")

        with open(output_file, "w") as file:
            file.write(decompressed)

        return output_file
