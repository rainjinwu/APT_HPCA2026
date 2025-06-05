import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches

import pandas as pd

data1 = pd.read_csv('general_height.data',sep='\t', header=None)

# Step 2: Extract the x and y values
x1 = data1.iloc[:, 0]  # First column
y1 = data1.iloc[:, 1]  # Second column
y2 = data1.iloc[:, 2]  # Third column
y3 = data1.iloc[:, 3]  # Fourth column
y4 = data1.iloc[:, 4]  # Fifth column

# Generate x-axis values (assuming each data point is one unit apart)
x = range(1, 10000)

# Plotting the line graph
plt.figure(figsize=(15, 8))
plt.plot(x1, y1, label="h_i+1=2", marker='o', linestyle='-', color='b', linewidth=3, zorder=2)
plt.plot(x1, y2, label="h_i+1=4", marker='^', linestyle='-', color='r', linewidth=3, zorder=2)
plt.plot(x1, y3, label="h_i+1=8", marker='*', linestyle='-', color='g', linewidth=3, zorder=2)
plt.plot(x1, y4, label="h_i+1=16", marker='s', linestyle='-', color='y', linewidth=3, zorder=2)

# plt.legend(fontsize=24, loc='lower center')
plt.legend(fontsize=24, loc='upper left')
plt.xlabel('Current Step Height (hi)', fontsize=24)
plt.ylabel('TRHD',fontsize=24)
plt.grid(True)
plt.tight_layout()

plt.yticks(range(0, 400, 50))  # Set ymin and ymax for the y-axis
# plt.xticks(range(1, 40, 5))  # Set xmin and xmax for the x-axis
plt.xscale('log')
plt.ylim(bottom=0) #确保y轴从0开始
plt.ylim(top=400)  #确保y轴在1100结束
plt.xlim(left=1)  # 确保x轴从0开始
# plt.xlim(right=10000)  # 确保x轴在10000结束
# plt.text(10.5, 65, 'Beyond DDR5 Specs', fontsize=18, rotation=0, color='black', weight='bold')
# plt.text(1.15, 65, 'Within DDR5 Specs', fontsize=18, rotation=0, color='black', weight='bold')

# plt.axvline(x=4, color='gray', linestyle='--', linewidth=3, zorder=1)  # Vertical dotted line at x=3
# arrow3 = patches.FancyArrowPatch((1, 63), (4, 63), mutation_scale=15, color='black', arrowstyle='<|-|>', linestyle='--', linewidth=2)
# plt.gca().add_patch(arrow3)

# arrow4 = patches.FancyArrowPatch((4, 63), (20, 63), mutation_scale=15, color='black', arrowstyle='<|-|>', linestyle='--', linewidth=2)
# plt.gca().add_patch(arrow4)

# Displaying the plot
plt.xticks(fontsize=24)
plt.yticks(fontsize=24)  
plt.grid(True)  # Show gridlines
plt.tight_layout()  # Adjust layout to avoid labels getting cut off
# plt.subplots_adjust(left=0.1)
plt.savefig('fig_hi.pdf', format='pdf')
plt.show()

