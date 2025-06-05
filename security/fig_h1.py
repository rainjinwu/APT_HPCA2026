import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches

import pandas as pd

data1 = pd.read_csv('proximate_row.data',sep='\t', header=None)

# Step 2: Extract the x and y values
x1 = data1.iloc[:, 0]  # First column
y1 = data1.iloc[:, 1]  # Second column
y2 = data1.iloc[:, 2]  # Third column
y3 = data1.iloc[:, 3]  # Fourth column
y4 = data1.iloc[:, 4]  # Fifth column
y5 = data1.iloc[:, 5]  # Sixth column
y6 = data1.iloc[:, 6]  # Seventh column
y7 = data1.iloc[:, 7]  # Eighth column
y8 = data1.iloc[:, 8]  # Ninth column


# Generate x-axis values (assuming each data point is one unit apart)
x = range(1, 40)

# Plotting the line graph
plt.figure(figsize=(16, 8))
plt.plot(x1, y1, label="h2=2", marker='o', linestyle='--', color='b', linewidth=3, zorder=2)
plt.plot(x1, y2, label="h2=2 w/ extra refresh", marker='o', linestyle='-', color='b', linewidth=3, zorder=2)
plt.plot(x1, y3, label="h2=4", marker='*', linestyle='--', color='g', linewidth=3, zorder=2)
plt.plot(x1, y4, label="h2=4 w/ extra refresh", marker='*', linestyle='-', color='g', linewidth=3, zorder=2)
plt.plot(x1, y5, label="h2=8", marker='^', linestyle='--', color='y', linewidth=3, zorder=2)
plt.plot(x1, y6, label="h2=8 w/ extra refresh", marker='^', linestyle='-', color='y', linewidth=3, zorder=2)
plt.plot(x1, y7, label="h2=16", marker='s', linestyle='--', color='r', linewidth=3, zorder=2)
plt.plot(x1, y8, label="h2=16 w/ extra refresh", marker='s', linestyle='-', color='r', linewidth=3, zorder=2)
# plt.legend(fontsize=24, loc='lower center')
plt.legend(fontsize=24, loc='upper left')
plt.xlabel('Step Height of Proximate Victim Rows (h1)', fontsize=24)
plt.ylabel('TRHD',fontsize=24)
plt.grid(True)
plt.tight_layout()

plt.yticks(range(0, 501, 50))  # Set ymin and ymax for the y-axis
plt.xticks(range(1, 21, 1))  # Set xmin and xmax for the x-axis
plt.ylim(bottom=0) #确保y轴从0开始
plt.ylim(top=500)  #确保y轴在1100结束
plt.xlim(left=1)  # 确保x轴从0开始
plt.xlim(right=20)  # 确保x轴在6结束
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
plt.savefig('fig_h1.pdf', format='pdf')
plt.show()

