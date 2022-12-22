import random

def generate_files(qtt_files, qtt_numbers):
  for i in range(qtt_files):
    filename = f"arquivo_{i}.txt"
    file_io = open(filename, "w")
    for _ in range(qtt_numbers):
      value = random.randint(-1000, 1000)
      file_io.write(f"{value} ")


if __name__ == "__main__":
  qtt_files = int(input("Qtd de arquivos: "))
  qtt_numbers = int(input("Qtd de números por arquivo: "))
  generate_files(qtt_files, qtt_numbers)