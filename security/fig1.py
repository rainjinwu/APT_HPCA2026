import numpy as np
import matplotlib.pyplot as plt

# 读取数据
filename = "fig1.data"
data = np.loadtxt(filename)

# 提取 X 轴数据（第一列）和 Y 轴数据（其余列）
x = data[:, 0]
y_values = data[:, 1:]

# 定义折线的标签和颜色
labels = ["BR=1", "BR=2", "BR=3", "BR=4", "BR=5", "BR=6"]
colors = ["b", "g", "r", "c", "m", "y"]
# markers = ['o', 's', '^', 'v', 'd', '*']

# 绘制折线图
plt.figure(figsize=(10, 4))
for i in range(y_values.shape[1]):
    plt.plot(x, y_values[:, i], label=labels[i], color=colors[i], marker='o', linestyle='-')

# 添加标签和图例
plt.xlabel("Max. Number of Rows in Subarray", fontsize=20)
plt.ylabel("MinTRH (TRH-D)", fontsize=20)
plt.legend(loc='upper left', fontsize=12, ncol=3)
plt.grid(True)

plt.yticks(range(0, 301, 50))  # Set ymin and ymax for the y-axis
plt.xticks(range(500, 1201, 100))  # Set xmin and xmax for the x-axis
plt.xlim(left=500)  # Set x-axis to start at 0
plt.xlim(right=1200)  # Set x-axis to end at 1200
plt.ylim(bottom=0)  # Set y-axis to start at 0

# Adjust layout to avoid labels getting cut off
plt.tight_layout()  # Adjust layout to avoid labels getting cut off
plt.savefig('fig1.pdf', format='pdf')
plt.show()