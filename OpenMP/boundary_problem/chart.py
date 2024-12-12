import matplotlib.pyplot as plt

def read_data(filename):
    with open(filename, 'r') as file:
        x_vals = [float(x) for x in file.readline().split(' ')[:-1]]
        y_vals = [float(y) for y in file.readline().split(' ')[:-1]]
    
    return x_vals, y_vals 

input_file = 'results.txt'


x, y = read_data(input_file)
plt.figure(figsize=(10, 6))
plt.plot(x, y, marker='o')
plt.xlabel('x')
plt.ylabel('y')
plt.title('y(x)')
plt.grid(True)
plt.show()
# plt.savefig("../results/" + input_file, format="png")
