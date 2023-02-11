# การป้องกันหน้า Dashboard Ui ของ node-red

สามารถทำได้ด้วยการแก้ไขไฟล์ setting.js 
โดยให้เตรียม สร้างรหัสผ่านด้วยคำสั่ง 

```
node-red-admin hash-pw
```
ระบุ password ที่ต้องการลงไป  ระบบจะแปลง password ที่ผ่านการเข้ารหัสแล้วออกมาให้
ให้ copy เก็บไว้ก่อน
จากนั้นให้เข้าไปที่เก็บ node-red ด้วยคำสั่ง cd แล้ว sudo nano settting.js ดังต่อไปนี้

```
cd ~/.node-red
sudo nano setting.js
```
เลือนไปแก้ไขบรรทัด  httpNodeAuth และ  httpStaticAuth ด้วยการแก้ไข username(ตั้งชื่อที่จะใช้ login) และ password(ที่เตรียมไว้) ดังรูป

![image](https://user-images.githubusercontent.com/37249027/218268985-58b7e759-c354-40f5-a621-1cbb93481d6d.png)

บันทึก แล้ว เริ่ม node-red ใหม่อีกครั้ง


# Last update 11 กพ 2565
เพิ่มการแสดงค่าความแรงสัญญาณ WiFi (RSSI) ของ ESP32 

# update 2/10/2565

ปรับ Redesign Flow ใหม่ ให้มีจุดที่ user ต้องเข้าไปแก้ไข Code น้อยลง


เพิ่มเติม ตัวอย่างการตั้งค่า timer ให้ ch3 ของทุกๆบอร์ด (A001-A004)


ฝั่ง Simulation
  ใส่ switch เพื่อปิดการจำลองการทำงานของบอร์ด ESP32 
         การทำงานของ simulator เพื่อเอาไว้สะดวกในการสร้างและแก้ไขฝั่ง dashboard โดยไม่ต้องมี ESP32 จะได้ไม่รบกวนเวลาใช้งานจริง
  แก้ไขสเกลของเซนเซอร์ความชื้นในดิน  จาก 0-100 เป็น 0-4096 เพื่อให้สอดคล้องกับ sensor ที่ใช้
  
ฝั่ง Flow 
  แก้ไข ค่า ch เปลี่ยนมาใช้ int แทน boolean เนื่องจาก boolean ไม่ได้เป็นชนิดตัวแปรหลักของ Arduino จะได้หลีกเลี่ยงปัญหาในการไม่เข้ากันของ Lib ใน version ที่เก่ากว่า
  เพิ่ม การทำงานแบบอัตโนมัติ ด้วยการตั้งเวลา (อยู่ที่ ch3) 
  
เพิ่มการแสดงค่ากำลังส่งของ  EPS32


## การนำไปใช้

ต้องติดตั้ง Node-red Plug-in ให้ครบถ้วนก่อน

* node-red-contrib-ui-led         version
* node-red-contrib-line-notify    version 3.1.3

## การติดตั้ง plugin ใช้คำสั่ง 

* npm install node-red-contrib-ui-led
* npm install node-red-contrib-line-notify

## การนำเข้า Flow 
ให้ใช้คำสั่ง import flow
แล้ววาง code  จากไฟล์ flow.js ลงไป
