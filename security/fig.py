import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches

import pandas as pd

data1 = pd.read_csv('fig.data',sep='\t', header=None)

# Step 2: Extract the x and y values
x1 = data1.iloc[:, 0]  # First column
y1 = data1.iloc[:, 1]  # Second column

data2 = pd.read_csv('fig2.data',sep='\t', header=None)

# Step 2: Extract the x and y values
x2 = data2.iloc[:, 0]  # First column
y2 = data2.iloc[:, 1]  # Second column


# Generate x-axis values (assuming each data point is one unit apart)
x = range(1, 7)

# Plotting the line graph
plt.figure(figsize=(8, 4))
plt.plot(x2, y2, label="Fractal Mitigation", marker='s', linestyle='-', color='r', linewidth=3)
plt.plot(x1, y1, label="Uniform Mitigation", marker='o', linestyle='-', color='b', linewidth=3)
plt.legend(fontsize=18)
plt.xlabel('Blast Radius (BR)', fontsize=24)
plt.ylabel('MinTRH (TRH-D*)',fontsize=24)
plt.grid(True)
plt.tight_layout()

plt.yticks(range(0, 451, 50))  # Set ymin and ymax for the y-axis
plt.xticks(range(1, 7, 1))  # Set xmin and xmax for the x-axis
plt.ylim(bottom=0) #确保y轴从0开始
plt.ylim(top=450)  #确保y轴在450结束
plt.xlim(left=1)  # 确保x轴从0开始
plt.xlim(right=6)  # 确保x轴在6结束
# plt.text(100.1, 600, '(Multi-TREFI)', fontsize=18, rotation=0, color='red')

# plt.axvline(x=73, color='gray', linestyle='--')  # Vertical dotted line at x=3


# Displaying the plot
plt.xticks(fontsize=20)
plt.yticks(fontsize=20)  
plt.grid(True)  # Show gridlines
plt.tight_layout()  # Adjust layout to avoid labels getting cut off
# plt.subplots_adjust(left=0.1)
plt.savefig('fig.pdf', format='pdf')
plt.show()

