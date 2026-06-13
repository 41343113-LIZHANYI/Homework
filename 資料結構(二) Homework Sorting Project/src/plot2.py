import matplotlib.pyplot as plt

# 測資規模 n
n = [500, 1000, 2000, 3000, 4000, 5000]

# Worst-case 數據 (ms)
w_insert = [0.2703, 1.1602, 4.3181, 10.1749, 17.2094, 25.8687]
w_merge  = [0.0462, 0.1032, 0.2168, 0.3439, 0.4575, 0.6052]
w_quick  = [0.0567, 0.1019, 0.2735, 0.2795, 0.3487, 0.4369]
w_heap   = [0.1074, 0.1396, 0.3542, 0.4506, 0.6962, 0.7405]
w_comp   = [0.0420, 0.0929, 0.1998, 0.3054, 0.4082, 0.5341]

# Average-case 數據 (ms)
a_insert = [0.1406, 0.6135, 2.2960, 4.8889, 8.5367, 13.0953]
a_merge  = [0.0677, 0.1267, 0.2600, 0.3645, 0.5249, 0.6567]
a_quick  = [0.0398, 0.0893, 0.2018, 0.3162, 0.4252, 0.5205]
a_heap   = [0.0544, 0.1189, 0.2661, 0.4078, 0.5421, 0.7010]
a_comp   = [0.0661, 0.1100, 0.2257, 0.3220, 0.4565, 0.5506]

# 建立畫布與子圖 (1列2欄)，稍微拉寬一點讓圖表更清楚
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

# 繪製 Worst-case 圖表
ax1.plot(n, w_insert, marker='o', label='Insertion', color='tab:blue')
ax1.plot(n, w_merge,  marker='s', label='Merge',     color='tab:orange')
ax1.plot(n, w_quick,  marker='^', label='Quick',     color='tab:green')
ax1.plot(n, w_heap,   marker='d', label='Heap',      color='tab:red')
ax1.plot(n, w_comp,   marker='x', label='Composite', color='tab:purple', linestyle='--', linewidth=2)
ax1.set_title('Worst-case Runtimes')
ax1.set_xlabel('n (Data Size)')
ax1.set_ylabel('Time (ms)')
ax1.grid(True, linestyle=':', alpha=0.7)
ax1.legend()

# 繪製 Average-case 圖表
ax2.plot(n, a_insert, marker='o', label='Insertion', color='tab:blue')
ax2.plot(n, a_merge,  marker='s', label='Merge',     color='tab:orange')
ax2.plot(n, a_quick,  marker='^', label='Quick',     color='tab:green')
ax2.plot(n, a_heap,   marker='d', label='Heap',      color='tab:red')
ax2.plot(n, a_comp,   marker='x', label='Composite', color='tab:purple', linestyle='--', linewidth=2)
ax2.set_title('Average-case Runtimes')
ax2.set_xlabel('n (Data Size)')
ax2.set_ylabel('Time (ms)')
ax2.grid(True, linestyle=':', alpha=0.7)
ax2.legend()

# 調整排版並顯示/存檔
plt.tight_layout()
plt.savefig('sorting_performance_v2.png', dpi=300) # 提高解析度讓報告截圖更清楚
plt.show()