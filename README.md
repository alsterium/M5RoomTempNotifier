# M5RoomTempNotifier
M5Stackと温度センサで部屋の温度をLINE Notify APIで通知
![2020-01-06_01-03-58_303](https://user-images.githubusercontent.com/52233620/72905108-56803e80-3d73-11ea-8774-b183c1f98a12.jpeg)
# About
M5Stackに温度センサDHT12を接続して取得した温度をLINE Notify API経由で10分置きに温度と湿度をポストします。
本体側の液晶画面には温度・湿度・次の送信までの秒数・HTTPステータスコードと温度グラフが表示されます。
# Usage
RoomTemp.ino中の"WIFI_SSID"と"WIFI_PASS"に接続したいWiFiのSSID、パスワードを設定し、![ここ](https://notify-bot.line.me/)で発行したアクセストークンを"api_key"に記述することで、LINEに通知を送ることができます。