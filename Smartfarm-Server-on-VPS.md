![image](https://user-images.githubusercontent.com/37249027/218273460-1c18a18e-b4a5-4b00-b155-feb20d4cb7b7.png)

 
## On Virtual Private Server:
## Smart Farm Server Installation 
## (คู่มือฉบับย่อ)


 | Authored by Tawan Phurat <br> Digital System Engineering Lab <br> Siam University <br> Version 1.01 | ![image](https://user-images.githubusercontent.com/37249027/218273504-f589e290-0608-45a8-902a-a9ecec704975.png)  |
| -------- | -------- |

การเซ็ตเครื่อง Virtual Private Server ที่ติดตั้งระบบปฏิบัติการ Ubuntu Server 20.04-LTS เพื่อให้ทำหน้าที่เป็นเครื่องแม่ข่าย (Server) สำหรับระบบสมาร์ทฟาร์มดังรูปที่ 1  ประกอบด้วยการติดตั้งโปรแกรมย่อยๆได้แก่ โปรแกรม webmin สำหรับใช้จัดการเครื่อง server ผ่านทาง web browser ในรูปแบบกราฟฟิค (GUI) โปรแกรม  Mosquitto ซึ่งทำหน้าที่เป็น MQTT Broker โปรแกรม NPM ซึ่งเป็นโปรแกรมบริหารจัดการคลังโปรแกรมของจาวาสคริปต์  โปรแกรม Node-Red และ  Node-Red Dashboard สำหรับนำไปใช้สร้างระบบจัดการฟาร์มอัจฉริยะ โดยจะนำเสนอไปทีละขั้นตอนดังต่อไปนี้
| ![image](https://user-images.githubusercontent.com/37249027/219899900-5cbf238b-282d-4a11-a07d-d19354315b3e.png) |
|---|
<p style="text-align: center;">รูปที่ 1 ระบบสมาร์ทฟาร์ม</p>

**หมายเหตุ**  กรณีที่ใช้ VPS ของผู้ให้บริการที่ติดตั้งระบบปฎิบัติการ linux (Ubuntu) server มาให้แล้ว สามารถดำเนินการต่อได้ทันที แต่ถ้าไม่ใช่กรณีนี้เช่น ต้องการลงระบบปฏิบัติการ linux server เองให้ไปดูในเอกสารเรื่องนี้โดยเฉพาะแล้วค่่อยกลับมาทำขั้นตอนต่อไป


  เมื่อได้รับ VPS account จากทางผู้ให้บริการ สิ่งที่ต้องทำคือเชื่อมต่อเข้าไปที่เครื่อง VPS ทำการ Sing-in ในครั้งแรกแล้วทำการเปลี่ยน password 

* ระบบ mac สามารถเชื่อมต่อไปยัง VPS ได้โดยตรงด้วย Terminal ของ mac ให้ทำการ sign in เข้าระบบ แล้วไปที่หัวข้อขั้นตอนการเปลี่ยน password
* ระบบ window ให้ใช้โปรแกรม putty ในการเชื่อมต่อไปยัง  VPS  หากไม่มีโปรแกรม putty ให้ทำการ download โปรแกรมจาก https://putty.org



