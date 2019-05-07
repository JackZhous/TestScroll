# 垂直滑动方案布局性能测试记录

##  ScrollView

### 时间毫秒计算

多次计算值
onWindowFocusChanged - onCreate

__热启动：__
80、106、106、103、102、112、114

__冷启动：__
209、202、203、234、240、242、228

trace文件，也是在上面两个方法之间：
类型名+启动方式+启动时长

## RecycleView

多次计算值
onWindowFocusChanged - onCreate 

__热启动：__
110、103、99、98、103

__冷启动：__
231、233、222、221、230

trace文件同上，在当前路径下

## 目的

利用Android device monitor和trace工具，深度定位每一个步骤执行效率，已确定出最终的方案


