
/*// 问题
* 1.box2鼠标边界问题暂时没有解决 ??
* 2.下载功能未完全实现，Base64格式转换 ？？？
* 3.reset无法改图片位置和大小？？？？？？？？
*
*/




// 生成UUID的函数
function generateUUID() {
    return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
      var r = Math.random() * 16 | 0,
          v = c === 'x' ? r : (r & 0x3 | 0x8);
      return v.toString(16);
    });
}

const uuid = generateUUID();  /////////////////////////
const saveFilename = uuid;


function info(id, val) {
    this.id = id;
    this.val = val;
}






let hashMap = {
    "btn1_1": { flag: 1, canInput:true },
    "btn1_2": { flag: 1, canInput: true },
    "btn1_3": { flag: 1, canInput: true },
    "btn2_1": { flag: 1, canInput: false },
    "btn2_2": { flag: 1, canInput: false },
    "btn2_3": { flag: 1, canInput: false },
    "btn2_4": { flag: 1, canInput: false },
    "btn3_1": { flag: 1, canInput: false },
    "btn3_2": { flag: 1, canInput: false },
    "btn4_1": { flag: 1, canInput: false },
    "btn4_2": { flag: 1, canInput: false},
    "btn4_3": { flag: 1, canInput: false},
};






function AddButtonEvent() {



    let keyArr = Object.keys(hashMap);

    for (let i = 0; i < keyArr.length; i++) {

        let id = keyArr[i];
        let mySpan = document.getElementById(id);

        mySpan.addEventListener('click', () => {

            let newImg = mySpan.querySelector("img");
            if (hashMap[id].flag == 1) {
                // 获取模板
                const template = document.querySelector('#testbox');

                // 克隆模板内容并填充数据
                const instance = document.importNode(template.content, true);
                let container = document.querySelector('#box3');

                let tempID = "box3" + "_" + id;
                instance.querySelector("div").id = tempID;
                instance.querySelector("p").textContent = mySpan.textContent;


                // 设置input
                if (hashMap[id].canInput == false) {
                    instance.querySelector("input[type=\"text\"]").disabled = true;
                }

                container.appendChild(instance);
                newImg.src = "../img/box2.png";
            }
            else {

                var element = document.getElementById("box3" + "_" + id);
                element.parentNode.removeChild(element);


                newImg.src = "../img/box1.png";
            }
            hashMap[id].flag = -hashMap[id].flag;


            // console.log("asdad");
        })
    }

}


let previousSrc = document.querySelector("#box2 img").src;

document.getElementById("btn_run").addEventListener("click", () => {
   
    let requestData = {
        "length": "0",
        "Data": [],
        "loadFilename":"",    /////////////
        "saveFilename":saveFilename
    }

    let len = 0;

    let minBoxArr = document.querySelectorAll("#box3 .minbox");
    // console.log(minBoxArr);
    if (minBoxArr.length == 0) {
        alert("还未添加任何算法，程序无法运行!");
        return;
    }

    for (let i = 0; i < minBoxArr.length; i++) {

        let id = minBoxArr[i].id;
        let myInputText = minBoxArr[i].querySelector("input[type=\"text\"]");
        let val = myInputText.value;

        let pattern = /^-?\d+(\.\d+)?$/;
        if (pattern.test(val) == false && hashMap[id.slice(5)].canInput == true) {
            alert(minBoxArr[i].innerText + "算法,参数输入错误!");
            return;
        } else {

            let tempData = {
                funcName: id,
                parameter: val
            };

            requestData["Data"].push(tempData);
            ++len;
        }
        // console.log(myInputText);
        // console.log(myInputText.value);
        // console.log(minBoxArr[i]); 
    }
    requestData["length"] = len.toString();

    let box2Img = document.querySelector("#box2 img");

    let filename = previousSrc.substring(previousSrc.lastIndexOf('/')); // 获取文件名
    requestData["loadFilename"] =filename;

    console.log(requestData);

    // 把数据发给服务器
    let xhr = new XMLHttpRequest(); // 创建一个新的 XHR 对象
    // xhr.open('POST', 'http://localhost:8888/index/index.json'); // 配置 XHR 对象

    xhr.open('POST', urlPath + "/cv/src"); // 配置 XHR 对象

    xhr.setRequestHeader('Content-Type', 'application/json'); // 声明请求头部类型 
    
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {

            // 将时间戳参数附加到 URL 后。强制浏览器忽略缓存并向服务器发送新请求以获取最新图片资源
            var timestampParam = '?nocache=' + new Date().getTime();
            box2Img.src = "../temp/" + saveFilename + ".bmp" + timestampParam;
        }
    };


    xhr.send(JSON.stringify(requestData)); // 将 JavaScript 对象转换成 JSON 字符串并发送请求



    // let xhr2 = new XMLHttpRequest(); // 创建一个新的 XHR 对象
    // xhr2.open('GET', 'http://localhost:8888/index/testimg1_t.bmp'); // 配置 XHR 对象
    // xhr2.setRequestHeader('Content-Type', 'image/bmp'); // 声明请求头部类型 
    // xhr2.send(JSON.stringify(requestData)); // 将 JavaScript 对象转换成 JSON 字符串并发送请求

    // previousSrc = box2Img.src;  // 保存处理之间前的图片，用于还原    ////////////////


    


    // box2Img.style.width="40%";   // 无法改位置和大小？？？？？？？？
    // box2Img.style.left="30%";
    // box2Img.style.top='5%';
});


const btn = document.getElementById("btn_add");
const selectedImage = document.querySelector("#box2 img");

btn.addEventListener("click", function() {
    const input = document.createElement('input');
    input.type = 'file';

    input.onchange = e => { 
        const file = e.target.files[0];
        if (!file.type.startsWith('image/')){ return }

        const reader = new FileReader();
        reader.onload = e => {
            selectedImage.src = e.target.result;

            // 要解析base64编码
            // // 创建一个新的 XHR 对象
            // const xhr = new XMLHttpRequest();
            // xhr.open('POST', urlPath + "/cv/src?save=img");
            // xhr.setRequestHeader('Content-Type', 'image/bmp');

            // // 设置响应处理函数
            // xhr.onreadystatechange = function () {
            //     if (xhr.readyState === 4 && xhr.status === 200) {
            //         // 请求成功，处理响应
            //         console.log(xhr.responseText);
            //     } else {
            //         // 请求失败或者未完成
            //         console.error('There was a problem with the request.');
            //     }
            // };
            // // 发送 BMP 图片数据
            // xhr.send(e.target.result);

        }
        reader.readAsDataURL(file); 
    }
    input.click()
});


/*
const btn = document.getElementById("btn_add");
const selectedImage = document.querySelector("#box2 img");

btn.addEventListener("click", function() {
    const input = document.createElement('input');
    input.type = 'file';

    input.onchange = e => { 
        const file = e.target.files[0];  // 获得上传得文件列表FileList对象中的第一个文件
        if (!file.type.startsWith('image/')){ 
            return;
        }

        const reader = new FileReader();   // 创建文件阅读器，用于格式转换
        let tempSrc;
        reader.onload = e => {    // 添加onload事件，表示当资源加载完成后自动触发
            tempSrc = e.target.result;  // 获得格式转换后文件的url

            let canvas = document.createElement("canvas"); 
            const image = new Image();

            image.onload = function() {
                canvas.width = image.width; 
                canvas.height = image.height;  
                canvas.getContext("2d").drawImage(image, 0, 0);  

                tempSrc = canvas.toDataURL('image/bmp');
                selectedImage.src = tempSrc;
            };
            image.src = tempSrc;
        }
        reader.readAsDataURL(file);     // 转成Base64编码格式并生成url，结果会自动存在 e.target.result中
    }

    input.click();  // 打开文件选择对话框
});
 */

document.getElementById("btn_reset").addEventListener("click", function() {
    // 改图片
    // 改缩放的比例
    // 改位置

    selectedImage.src = previousSrc;
});

let imgs = ["testimg1.bmp","testimg2.bmp","testimg3.bmp","testimg4.bmp"];
let num = 1;
document.getElementById("btn_exchange").addEventListener("click", function() {
    
    if(num == 4){
        num -= 4;
    }
    selectedImage.src ="../img/" + imgs[num%4];
    previousSrc = selectedImage.src;
    num++;
});




document.getElementById("btn_downLoad").addEventListener("click", function() {

  const link = document.createElement('a');

  link.href = selectedImage.src;
  // 给这个下载的链接添加 "下载" 属性和文件名称
  link.download = selectedImage.src + ".bmp";

  // 插入到 DOM 中来触发下载
  document.body.appendChild(link);

  // 触发链接的点击事件去执行下载
  link.click();

  // 将链接从 DOM 中移除
  document.body.removeChild(link);
});


// ================================================================================================


let container = document.querySelector('#box2');
let image = container.querySelector('#box2 img');

let currentScale = 1;         // 当前比例1为正常
let minScale = 0.3;           // 最小缩放比例
let currentTranslateX = 0;
let currentTranslateY = 0;

// 图片缩放和移动的操作函数
function updateTransform() {
    let transformCSS =
      `
  translate(${currentTranslateX}px, ${currentTranslateY}px)     
  scale(${currentScale})
`;
    image.style.transform = transformCSS;
    /*
    * 1.模板字符串用反引号``括起来，在内部可以用 ${变量名} 这种形式直接插入变量。
    * 2.translate()位移变换函数，一个重载版本为translate(dx,dy)。
    * 3.scale()缩放变换函数,一个重载版本为scale(n),n=1时表示不缩放。
    */
}

container.addEventListener('wheel', (event) =>{
    event.preventDefault();   
});


// 图片缩放的事件函数
image.addEventListener('wheel', (event) => {
    event.preventDefault();      // 阻止默认的滚轮事件，即避免页面滚动或缩放。
    image.style.transitionDuration = "0.3s";


    let delta = Math.sign(event.deltaY);    // 判断滚轮滚动的方向，大于0是向上滚动，小于0是向下滚动，Math.sign()是判断正负用的
    currentScale -= delta * 0.1;    // 每次缩放的比例为0.1
    currentScale = Math.max(currentScale, minScale);    // 设置最小缩放比例

    updateTransform();    // 更新数据进行变换
});



container.addEventListener('mousedown', (event) =>{
    event.preventDefault();   
});


// 图片移动的事件函数
image.addEventListener('mousedown', (event) => {

    event.preventDefault();   // 阻止默认的鼠标事件
    image.style.transitionDuration = "0s"; // 修改过度动画时间为0，即没有过度动画，提高实时性

    let startClientX = event.clientX;
    let startClientY = event.clientY;

    container.addEventListener('mousemove', dragImage);
    container.addEventListener('mouseup', () => {
      container.removeEventListener('mousemove', dragImage);
    });


    function dragImage(event) {

      let distanceX = event.clientX - startClientX;
      let distanceY = event.clientY - startClientY;

      currentTranslateX += distanceX;
      currentTranslateY += distanceY;

      startClientX = event.clientX;
      startClientY = event.clientY;

      updateTransform();
    }
});


function main() {
    AddButtonEvent();


}

main();



