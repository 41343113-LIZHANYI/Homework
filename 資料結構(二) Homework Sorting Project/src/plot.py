import matplotlib.pyplot as plt

# 測資規模 n
n = [500, 1000, 2000, 3000, 4000, 5000]

# Worst-case 數據 (ms)
w_insert = [0.3678, 1.3670, 5.5038, 11.2106, 17.4549, 26.5433]
w_merge = [0.0558, 0.1598, 0.3290, 0.3937, 0.4569, 0.6040]
w_quick = [0.0375, 0.0901, 0.1724, 0.2663, 0.3473, 0.5042]
w_heap = [0.0561, 0.1254, 0.2458, 0.3880, 0.5905, 0.7433]
w_comp = [0.0581, 0.1264, 0.2500, 0.4910, 0.4630, 0.6075]

# Average-case 數據 (ms)
a_insert = [0.1464, 0.6028, 2.3692, 5.2477, 8.9456, 13.8180]
a_merge = [0.0708, 0.1327, 0.2530, 0.3714, 0.5008, 0.6497]
a_quick = [0.0419, 0.0944, 0.1983, 0.3177, 0.5740, 0.5238]
a_heap = [0.0552, 0.2289, 0.2609, 0.4277, 0.5409, 0.7097]
a_comp = [0.0769, 0.1352, 0.2638, 0.4701, 0.5152, 0.6399]

# 建立畫布與子圖 (1列2欄)
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

# 繪製 Worst-case 圖表
ax1.plot(n, w_insert, marker='o', label='Insertion')
ax1.plot(n, w_merge, marker='s', label='Merge')
ax1.plot(n, w_quick, marker='^', label='Quick')
ax1.plot(n, w_heap, marker='d', label='Heap')
ax1.plot(n, w_comp, marker='x', label='Composite', linestyle='--')
ax1.set_title('Worst-case Runtimes')
ax1.set_xlabel('n (Data Size)')
ax1.set_ylabel('Time (ms)')
ax1.grid(True)
ax1.legend()

# 繪製 Average-case 圖表
ax2.plot(n, a_insert, marker='o', label='Insertion')
ax2.plot(n, a_merge, marker='s', label='Merge')
ax2.plot(n, a_quick, marker='^', label='Quick')
ax2.plot(n, a_heap, marker='d', label='Heap')
ax2.plot(n, a_comp, marker='x', label='Composite', linestyle='--')
ax2.set_title('Average-case Runtimes')
ax2.set_xlabel('n (Data Size)')
ax2.set_ylabel('Time (ms)')
ax2.grid(True)
ax2.legend()

# 調整排版並顯示/存檔
plt.tight_layout()
plt.savefig('sorting_performance.png')
plt.show()