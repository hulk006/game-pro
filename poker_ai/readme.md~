1、TCP协议：
输入：
poker ai 接受tcp协议的字符串格式：
 char ai_input[] = "poker_info;111111;hole,AD,2C;com_card,;current_bet,200; game_pool_,600;chip,4000;check_,0;connect,1;send_end";
{牌局标志位；信息id；手牌；公牌；当前下注；奖池；本金剩余；是否可以让牌;长短连接，1代表长；结束标志}
信息段之间用“；”隔开，信息段内用“，”隔开，信息段长度根据信息有一定的限制
第二个信息段为当前信息的id，返回的信息将和这个id相同

输出：

完整输出信息：poker_fcr;111111;2;send_end××××××××××××××
{决策标志位；信息id；决策；结束标志位}
	     2代表ai的决策
决策表：{"fold", "call","raise", "Check","all_in"};
0代表fold，1代表call，2代表raise，3代表Check，4代表all in


2、服务器监听端口:
#define PORT 2245
3、服务器的IP地址为：
本机的ip地址

4、已经写了守护进程，本程序在电脑上需要运行一次，否则电脑卡顿

5、buid
根据cmakelists.txt 建立工程
本工程只使用了c++/c的库不需要其他依赖项
6、使用
运行程序，或者poker_ai的可执行文件即可，服务器讲一直处于监听端口状态
等待客户端发送数据

