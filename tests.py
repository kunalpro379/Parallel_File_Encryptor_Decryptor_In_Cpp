import os
import string
import random

def generate_random_story(word_count=1000):
    words = [''.join(random.choices(string.ascii_lowercase, k=random.randint(3, 10))) for _ in range(word_count)]
    return ' '.join(words)

def makeFiles(path, num_files=1000):  # Create 1000 test files
    os.makedirs(path, exist_ok=True)
    os.chdir(path)
    for i in range(num_files):
        filename = f"test_{i+1}.txt"
        with open(filename, 'w') as file:  # Specify the filename and mode
            random_story = generate_random_story(1000)
            file.write(random_story)

if __name__ == "__main__":
    makeFiles("C:/Users/kunal/OneDrive/CODE FOR LIFE/BuildFromScratch/Parallel_File_Encryptor_Decryptor/test")