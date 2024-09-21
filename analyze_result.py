import pandas as pd

# 读取 CSV 文件
file_path = 'result/experimental_results.csv'
df = pd.read_csv(file_path)

# 查看读取的数据
# print(df)

is_enough = True
total_run_time = 0
valid_instance_count = 0  # 统计符合条件的实例数量

for i in range(20):
    instance_name = 'map_8by8_obst12_agents8_ex' + str(i) + ".yaml"
    # print(instance_name)

    # 筛选条件
    filtered_df = df[(df['instance'] == instance_name)
                     & (df['device'] == '12400F')
                     & (df['method'] == 'cbs-mine')
                     & (df['disappear_at_goal'] == 0)
                     & (df['rand_seed'] == -1)
                     ]

    # 检查筛选后的结果是否足够
    if len(filtered_df) < 5:
        print(instance_name, "freq not enough")
        is_enough = False
        break

if is_enough:
    for i in range(20):
        instance_name = 'map_8by8_obst12_agents8_ex' + str(i) + ".yaml"
        # print(instance_name)

        # 筛选条件
        filtered_df = df[(df['instance'] == instance_name)
                         & (df['device'] == '12400F')
                         & (df['method'] == 'cbs-mine')
                         & (df['disappear_at_goal'] == 0)
                         & (df['rand_seed'] == -1)
                         ]

        # 计算 run_time 字段的平均值
        avg_run_time = filtered_df['run_time'].mean()
        print(instance_name, f"Average run_time: {avg_run_time}")

        # 累加所有实例的平均 run_time
        total_run_time += avg_run_time
        valid_instance_count += 1

    # 计算所有实例的平均 run_time
    overall_avg_run_time = total_run_time / valid_instance_count
    print(f"Overall average run_time for ex0 to ex19: {overall_avg_run_time}")
    # Overall average run_time for ex0 to ex19: 39.90578464428572