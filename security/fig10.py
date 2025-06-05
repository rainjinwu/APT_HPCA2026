import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

data = pd.read_csv('fig10.data',sep='\t', header=None)

# Step 2: Extract the x and y values
x1 = data.iloc[:, 0]  # First column
y1 = data.iloc[:, 1]  # Second column


# Generate x-axis values (assuming each data point is one unit apart)
x = range(0, 150)

# Plotting the line graph
plt.figure(figsize=(12, 4))
plt.plot(x1, y1, marker='o', linestyle='-', color='b')
plt.xlabel('Number of Attack Rows (k)', fontsize=24)
plt.ylabel('MinTRH',fontsize=24)
plt.grid(True)
plt.tight_layout()

plt.xticks(range(0, 150, 20))  # Set xmin and xmax for the x-axis
plt.xlim(left=0)  # 确保x轴从0开始
plt.text(100.1, 600, '(Multi-TREFI)', fontsize=18, rotation=0, color='red')

plt.axvline(x=73, color='gray', linestyle='--')  # Vertical dotted line at x=3


# Displaying the plot
plt.xticks(fontsize=20)
plt.yticks(fontsize=20)  
plt.grid(True)  # Show gridlines
# plt.tight_layout()  # Adjust layout to avoid labels getting cut off
plt.subplots_adjust(left=0.1)
plt.savefig('fig10.pdf', format='pdf')
plt.show()

