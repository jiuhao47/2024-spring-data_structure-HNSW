# 讨论
## 跳表
- Layer 0 - full graph
- Layer 1 ... fast way
   - 有几层？
     - MAX为宏定义参数
   - 均匀硬币？
     - 1
     - $$ layer=\lfloor -\ln (unif(0,1))*maxLayer\rfloor$$
       - maxLayer 索引权重，>1，pos，<1，调整硬币均匀性
- 插入
  - 从full graph建立
    - 独立节点，每个节点都有真实数据填充
    - 查找m临近的入口：定义好√/随机
    - 下指指针
  - random order reverse order
-  
## 图片向量化
-  DataType?
   -  1
   -  malloc
   -  ??
-  距离
-  索引信息保留
   -  编号
   -  批处理
   -  映射