import matplotlib.pyplot as plt
import math


def read_specific_line(filename, line_number):
    try:
        with open(filename, "r") as file:
            for current_line, content in enumerate(file, start=1):
                if current_line == line_number:
                    return content.strip()  # 去除行尾的换行符
    except FileNotFoundError:
        print(f"File {filename} not found.")
        return None
    except Exception as e:
        print(f"An error occurred: {e}")
        return None


def calculate_average(values):
    if not values:
        return 0
    return sum(values) / len(values)


def get_average_search_time(filename):
    HNSW_search_time = []
    Brute_search_time = []
    recall_rate = []
    for i in range(0, 50):
        HNSW_search_time.append(float(read_specific_line(filename, 3 + i * 15)))
        Brute_search_time.append(float(read_specific_line(filename, 4 + i * 15)))
        recall_rate.append(float(read_specific_line(filename, 5 + i * 15)))
    return (
        calculate_average(HNSW_search_time),
        calculate_average(Brute_search_time),
        calculate_average(recall_rate),
    )


# 使用示例
L_M_Near = [2, 3, 5, 10, 15, 20]
dataset = [
    "flower",
    "logo",
    "aircraft",
    "food",
    "clothing",
]
filename = "/home/jiuhao/Workspace/2024-Spring/data_structure/Ex/projects/NSW/data/source/output_3_6_51_2.txt"

filePathRoot = "/home/jiuhao/Workspace/2024-Spring/data_structure/Ex/projects/NSW/data/output/output_"
suffix = ".txt"
HNSW_search_time_average = []
Brute_search_time_average = []
recall_rate_average = []
build_time_average = []
for datasetIndex in range(2, 7):
    y1_HNSW_search_time = []
    y2_Brute_search_time = []
    y3_recall_rate = []
    y4_build_time = []
    for L_M_Near_Index in range(len(L_M_Near)):
        HNSW_search_time = []
        Brute_search_time = []
        recall_rate = []
        build_time = []
        for round in range(0, 3):
            filename = (
                filePathRoot
                + str(L_M_Near[L_M_Near_Index])
                + "_"
                + str(datasetIndex)
                + "_"
                + "51_"
                + str(round)
                + suffix
            )
            HNSW_search_time_temp, Brute_search_time_temp, recall_rate_temp = (
                get_average_search_time(filename)
            )
            build_time.append(float(read_specific_line(filename, 1)))
            HNSW_search_time.append(HNSW_search_time_temp)
            Brute_search_time.append(Brute_search_time_temp)
            recall_rate.append(recall_rate_temp)
        y1_HNSW_search_time.append(calculate_average(HNSW_search_time))
        y2_Brute_search_time.append(calculate_average(Brute_search_time))
        y3_recall_rate.append(calculate_average(recall_rate))
        y4_build_time.append(calculate_average(build_time))

    HNSW_search_time_average.append(y1_HNSW_search_time)
    Brute_search_time_average.append(y2_Brute_search_time)
    recall_rate_average.append(y3_recall_rate)
    build_time_average.append(y4_build_time)

    fig, ax1 = plt.subplots()
    ax2 = ax1.twinx()
    ax1.set_ylim(0, max(y1_HNSW_search_time) + 0.5)
    ax2.set_ylim(0, max(y1_HNSW_search_time) + 0.5)
    ax1.plot(L_M_Near, y1_HNSW_search_time, "r-")
    ax2.plot(L_M_Near, y2_Brute_search_time, "b-")
    ax1.set_xlabel("Connectivity")
    ax1.set_ylabel("HNSW Search Time(s)", color="r")
    ax2.set_ylabel("Brute Search Time(s)", color="b")
    plt.title("Dataset " + dataset[datasetIndex - 2])
    plt.savefig(
        "/home/jiuhao/Workspace/2024-Spring/data_structure/Ex/projects/NSW/data/pic/"
        + dataset[datasetIndex - 2]
        + "_time.jpg"
    )
    plt.close(fig)
    fig, ax3 = plt.subplots()
    ax3.plot(L_M_Near, y3_recall_rate, "g-")
    ax3.set_xlabel("Connectivity")
    ax3.set_ylabel("Recall Rate", color="g")
    plt.title("Dataset " + dataset[datasetIndex - 2])
    plt.savefig(
        "/home/jiuhao/Workspace/2024-Spring/data_structure/Ex/projects/NSW/data/pic/"
        + dataset[datasetIndex - 2]
        + "_recall.jpg"
    )
    plt.close(fig)
    fig, ax4 = plt.subplots()
    ax4.plot(L_M_Near, y4_build_time, "y-")
    ax4.set_xlabel("Connectivity")
    ax4.set_ylabel("Build Time(s)", color="y")
    plt.title("Dataset " + dataset[datasetIndex - 2])
    plt.savefig(
        "/home/jiuhao/Workspace/2024-Spring/data_structure/Ex/projects/NSW/data/pic/"
        + dataset[datasetIndex - 2]
        + "_build.jpg"
    )
    plt.close(fig)

build_time_average_all = []
L_M_Near_nlogn = [math.log(i) for i in L_M_Near]
for j in range(6):
    sum = 0
    for i in range(5):
        sum = sum + build_time_average[i][j]
    build_time_average_all.append(sum / 5)
fig, ax5 = plt.subplots()
ax5.plot(L_M_Near, build_time_average_all, "y-")
ax5.set_xlabel("Connectivity")
ax5.set_ylabel("Build Time(s)")
plt.title("Dataset Average")
plt.savefig(
    "/home/jiuhao/Workspace/2024-Spring/data_structure/Ex/projects/NSW/data/pic/"
    + "average_build.jpg"
)
plt.close(fig)

HNSW_search_time_average_all = []
Brute_search_time_average_all = []
L_M_Near_nlogn = [math.log(i) for i in L_M_Near]
for j in range(6):
    sum1 = 0
    sum2 = 0
    for i in range(5):
        sum1 = sum1 + HNSW_search_time_average[i][j]
        sum2 = sum2 + Brute_search_time_average[i][j]
    HNSW_search_time_average_all.append(sum1 / 5)
    Brute_search_time_average_all.append(sum2 / 5)
fig, ax6 = plt.subplots()
ax7 = ax6.twinx()
ax6.set_ylim(0, max(HNSW_search_time_average_all) + 0.5)
ax7.set_ylim(0, max(HNSW_search_time_average_all) + 0.5)
ax6.plot(L_M_Near, HNSW_search_time_average_all, "r-")
ax7.plot(L_M_Near, Brute_search_time_average_all, "b-")
ax6.set_xlabel("Connectivity")
ax6.set_ylabel("HNSW Search Time(s)", color="r")
ax7.set_ylabel("Brute Search Time(s)", color="b")
plt.title("Dataset Average")
plt.savefig(
    "/home/jiuhao/Workspace/2024-Spring/data_structure/Ex/projects/NSW/data/pic/"
    + "average_time.jpg"
)
plt.close(fig)

recall_rate_average_all = []
for j in range(6):
    sum = 0
    for i in range(5):
        sum = sum + recall_rate_average[i][j]
    recall_rate_average_all.append(sum / 5)
fig, ax8 = plt.subplots()
ax8.plot(L_M_Near, recall_rate_average_all, "g-")
ax8.set_xlabel("Connectivity")
ax8.set_ylabel("Recall Rate", color="g")
plt.title("Dataset Average")
plt.savefig(
    "/home/jiuhao/Workspace/2024-Spring/data_structure/Ex/projects/NSW/data/pic/"
    + "average_recall.jpg"
)
plt.close(fig)
