import math
import heapq
from collections import defaultdict

# Function to sort the probability map in descending order
def sort_probabilities(prob_map):
    return sorted(prob_map.items(), key=lambda x: x[1], reverse=True)

# Function to convert frequency counts to probabilities
def to_prob(s, total_chars):
    for char in s:
        s[char] = s[char] / total_chars
    return s

# Function to convert integer to a binary string with fixed length
def int_to_binary_string(code, n):
    return format(code, f"0{n}b")

# Function to update Tunstall codebook (mapping symbols to binary codes)
def update_tunstall_table(s, tunstall_codebook, n):
    pq = []
    # Insert all the symbols into the priority queue
    for symbol, prob in s:
        heapq.heappush(pq, (prob, symbol))
    
    code = 0
    while len(tunstall_codebook) < (1 << n):
        prob, top = heapq.heappop(pq)
        if top not in tunstall_codebook:
            tunstall_codebook[top] = int_to_binary_string(code, n)
            code += 1
        for symbol, prob in s:
            new_seq = top + symbol
            new_prob = prob * s[symbol]
            heapq.heappush(pq, (new_prob, new_seq))

# Function to encode the input data using the Tunstall codebook
def encode(input_data, tunstall_codebook):
    encoded_data = ''.join([tunstall_codebook[char] for char in input_data])
    return encoded_data

# Function to decode the encoded data using the Tunstall codebook
def decode(encoded_data, tunstall_codebook):
    reverse_codebook = {v: k for k, v in tunstall_codebook.items()}
    decoded_data = ''
    current_code = ''
    for bit in encoded_data:
        current_code += bit
        if current_code in reverse_codebook:
            decoded_data += reverse_codebook[current_code]
            current_code = ''
    return decoded_data

# Function to implement the Tunstall coding algorithm
def tunstall(input_file, mode):
    with open(input_file, 'r') as file:
        data = file.read()

    # Calculate frequency of each character in the input data
    freq_map = defaultdict(int)
    for char in data:
        freq_map[char] += 1
    total_chars = len(data)
    
    # Convert frequency counts to probabilities
    prob_map = to_prob(freq_map, total_chars)

    # Sort symbols by probability in descending order
    sorted_prob_map = sort_probabilities(prob_map)
    
    # Calculate the number of bits required for the codebook
    n = math.ceil(math.log2(len(sorted_prob_map)))

    # Generate the Tunstall codebook
    tunstall_codebook = {}
    update_tunstall_table(sorted_prob_map, tunstall_codebook, n)
    
    # Encode the data
    if mode == 'compress':
        encoded_data = encode(data, tunstall_codebook)
        return encoded_data, tunstall_codebook  # Return encoded data and the codebook

    # Decode the data
    elif mode == 'decompress':
        decoded_data = decode(data, tunstall_codebook)
        return decoded_data  # Return the decoded data