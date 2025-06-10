import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches

import pandas as pd

data1 = pd.read_csv('fractal_mitigation.data',sep='\t', header=None)

# Step 2: Extract the x and y values
x1 = data1.iloc[:, 0]  # First column
y1 = data1.iloc[:, 1]  # Second column

data2 = pd.read_csv('step_mitigation.data',sep='\t', header=None)

# Step 2: Extract the x and y values
x2 = data2.iloc[:, 0]  # First column
y2 = data2.iloc[:, 1]  # Second column


# Generate x-axis values (assuming each data point is one unit apart)
x = range(1, 27)

# Plotting the line graph
plt.figure(figsize=(20, 5))
plt.plot(x1, y1, label="MINT+Fractal Mitigation", marker='o', linestyle='-', color='b', linewidth=3, zorder=2)
plt.plot(x2, y2, label="APT+Step Mitigation", marker='s', linestyle='-', color='r', linewidth=3, zorder=2)
plt.legend(fontsize=24, loc='lower center')
plt.xlabel('Blast Radius (BR)', fontsize=24)
plt.ylabel('Avg. ACTs per BR Row',fontsize=24)
plt.grid(True)
plt.tight_layout()

plt.yticks(range(0, 81, 10))  # Set ymin and ymax for the y-axis
plt.xticks(range(1, 21, 1))  # Set xmin and xmax for the x-axis
plt.ylim(bottom=0) #确保y轴从0开始
plt.ylim(top=80)  #确保y轴在1100结束
plt.xlim(left=1)  # 确保x轴从0开始
plt.xlim(right=20)  # 确保x轴在6结束
plt.text(10.5, 73, 'Beyond DDR5 Specs', fontsize=18, rotation=0, color='black', weight='bold')
plt.text(1.15, 73, 'Within DDR5 Specs', fontsize=18, rotation=0, color='black', weight='bold')

plt.axvline(x=4, color='gray', linestyle='--', linewidth=3, zorder=1)  # Vertical dotted line at x=3
arrow3 = patches.FancyArrowPatch((1, 70), (4, 70), mutation_scale=15, color='black', arrowstyle='<|-|>', linestyle='--', linewidth=2)
plt.gca().add_patch(arrow3)

arrow4 = patches.FancyArrowPatch((4, 70), (20, 70), mutation_scale=15, color='black', arrowstyle='<|-|>', linestyle='--', linewidth=2)
plt.gca().add_patch(arrow4)

# Displaying the plot
plt.xticks(fontsize=24)
plt.yticks(fontsize=24)  
plt.grid(True)  # Show gridlines
plt.tight_layout()  # Adjust layout to avoid labels getting cut off
# plt.subplots_adjust(left=0.1)
plt.savefig('fig_blast_radius.pdf', format='pdf')
plt.show()

