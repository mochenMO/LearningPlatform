var currentPage = 1; // 当前页码
var pageSize = 10; // 每页显示的记录数


let userData;


function getUserData() {
    console.log("getUserData");

    // 向服务器请求用户的信息
    var xhr = new XMLHttpRequest();
    xhr.open("GET", urlPath + "/main/src?get=userData", true);
    xhr.responseType = 'json';
    xhr.send();

    // 请求完成时的处理函数(异步运行)
    xhr.onload = function () {
        if (xhr.status == 200) {
            // let userInfo = xhr.response;
            // window.userInfo = userInfo;
            // console.log("userData:", userInfo);
            // console.log("Global userData:", window.userInfo);
            userData = xhr.response;
            console.log("userData:", userData); ///////////////////////////
            if (userData != null) {
                document.querySelector('.username-box').textContent = userData["uaccount"];
            }
        } else {
            console.error("Request failed. Status:", xhr.status);
        }
    };
}


// 定义函数将模板内容显示到左侧盒子中
function showLeftBox01() {
    // 获取模板元素
    const templateBox = document.querySelector(".template-user-info-box");
    // 获取左侧盒子元素
    const leftBox = document.querySelector(".left-box");

    // 清空左侧盒子内容
    leftBox.innerHTML = '';

    // 将模板元素的内容添加到左侧盒子中
    leftBox.appendChild(templateBox.content.cloneNode(true));

    if (userData != null) {
        document.querySelector('.username-input-box:nth-of-type(1) input').placeholder = userData["uaccount"];
        document.querySelector('.username-input-box:nth-of-type(2) input').placeholder = userData["upassword"];
        document.querySelector('.username-input-box:nth-of-type(3) input').placeholder = userData["uphone"];
        document.querySelector('.username-input-box:nth-of-type(4) input').placeholder = userData["uemail"];
        document.querySelector('.username-input-box:nth-of-type(5) input').placeholder = userData["uage"];
    }



    document.querySelector('.headshot-box').addEventListener('click', function () {
        console.log("back22 ok");

    });

    document.querySelector('.btn1-box').addEventListener('click', function () {
        // 获取所有输入框的值

        if (window.confirm("确认要修改吗？") == true) {

            var uaccountInput = document.querySelector('.username-input-box:nth-of-type(1) input');
            var upasswordInput = document.querySelector('.username-input-box:nth-of-type(2) input');
            var uphoneInput = document.querySelector('.username-input-box:nth-of-type(3) input');
            var uemailInput = document.querySelector('.username-input-box:nth-of-type(4) input');
            var uageInput = document.querySelector('.username-input-box:nth-of-type(5) input');

            var userInfo = {
                uaccount: uaccountInput.placeholder,
                upassword: upasswordInput.placeholder,
                uphone: uphoneInput.placeholder,
                uemail: uemailInput.placeholder,
                uage: uageInput.placeholder
            };


            if (uaccountInput.value != "") {
                userInfo["uaccount"] = uaccountInput.value;
                uaccountInput.placeholder = uaccountInput.value;
            }
            if (upasswordInput.value != "") {
                userInfo["upassword"] = upasswordInput.value;
                upasswordInput.placeholder = upasswordInput.value;
            }
            if (uphoneInput.value != "") {
                userInfo["uphone"] = uphoneInput.value;
                uphoneInput.placeholder = uphoneInput.value;
            }
            if (uemailInput.value != "") {
                userInfo["uemail"] = uemailInput.value;
                uemailInput.placeholder = uemailInput.value;
            }
            if (uageInput.value != "") {
                userInfo["uage"] = uageInput.value;
                uageInput.placeholder = uageInput.value;
            }

            userData = userInfo;


            // 将 JSON 对象发送给服务器
            var jsonData = JSON.stringify(userInfo);
            var xhr = new XMLHttpRequest();
            xhr.open('POST', urlPath + '/main/src?save=userData', true);
            xhr.setRequestHeader('Content-Type', 'application/json');

            xhr.onreadystatechange = function () {
                if (xhr.readyState === XMLHttpRequest.DONE) {
                    if (xhr.status === 200) {
                        // 请求成功，处理响应数据
                        console.log('ok');
                    } else {
                        // 请求失败，输出错误信息
                        console.error('发生错误:', xhr.status);
                    }
                }
            };
            // 发送请求
            xhr.send(jsonData);
        }
    });

    document.querySelector('.btn2-box').addEventListener('click', function () {
        if (window.confirm("确定退出登录吗？") == true) {
            var cookies = document.cookie.split(";");
            for (var i = 0; i < cookies.length; i++) {
                var cookie = cookies[i];
                var eqPos = cookie.indexOf("=");
                var name = eqPos > -1 ? cookie.substr(0, eqPos) : cookie;
                document.cookie = name + "=;expires=Thu, 01 Jan 1970 00:00:00 GMT;path=/";
            }

            // 重定向到登录页面
            window.location.replace(urlPath + "/login");
        }
    });

    document.querySelector('.btn3-box').addEventListener('click', function () {
        showLeftBox02();
    });
}



// 定义函数将模板内容显示到左侧盒子中
function showLeftBox02() {
    // 获取模板元素
    const templateBox = document.querySelector(".template-user-box");
    // 获取左侧盒子元素
    const leftBox = document.querySelector(".left-box");

    // 清空左侧盒子内容
    leftBox.innerHTML = '';

    // 将模板元素的内容添加到左侧盒子中
    leftBox.appendChild(templateBox.content.cloneNode(true));


    document.querySelector('.headshot-box').addEventListener('click', function () {
        showLeftBox01();
    });

    document.querySelector('.btn1-box').addEventListener('click', function () {
        console.log("showLeftBox02 btn1_boxbtn1_box ok");
    });



    if (userData == null) {
        getUserData();
    }
    else {
        document.querySelector('.username-box').textContent = userData["uaccount"];
    }


    // // 向服务器请求用户的信息
    // var xhr = new XMLHttpRequest();
    // xhr.open("GET", urlPath + "/main/src?get=userData", true);
    // xhr.responseType = 'json';
    // xhr.send();

    // // 请求完成时的处理函数
    // xhr.onload = function() {
    //     if (xhr.status == 200) {
    //         // let userInfo = xhr.response;
    //         // window.userInfo = userInfo;
    //         // console.log("userData:", userInfo);
    //         // console.log("Global userData:", window.userInfo);

    //         userData = xhr.response;
    //         console.log("userData:", userData); ///////////////////////////
    //         if(userData != null){
    //             document.querySelector('.username-box').textContent = userData["uaccount"];
    //         }
    //     } else {
    //         console.error("Request failed. Status:", xhr.status);
    //     }
    // };





    // 给用户名框元素添加文字
    // usernameBox.textContent = userData.uaccout;
}



// 置顶功能
function topButtonEvent() {
    const scrollTopBtn = document.querySelector(".top-box");
    // 添加点击事件监听器
    scrollTopBtn.addEventListener("click", () => {
        // 平滑滚动到页面顶部
        window.scrollTo({
            top: 0,
            behavior: "smooth"
        });
    });

    // // 监听页面滚动事件
    // window.addEventListener("scroll", () => {
    //   // 如果页面滚动超过 500px，显示置顶按钮，否则隐藏
    //   if (window.scrollY > 500) {
    //     scrollTopBtn.style.display = "block";
    //   } else {
    //     scrollTopBtn.style.display = "none";
    //   }
    // });

}










// 搜索功能
function searchBlog() {
    // var xhr = new XMLHttpRequest();
    // xhr.open('GET', 'http://127.0.0.1:8888/users?page=' + currentPage + '&size=' + pageSize, true);
    // xhr.onreadystatechange = function() {
    //     if(xhr.readyState == 4 && xhr.status == 200) {
    //         var users = JSON.parse(xhr.responseText);
    //         displayUserTable(users);
    //     }
    // };
    // xhr.send();

}

function convertToUTF8(str) {
    var decoder = new TextDecoder('ASCII');
    var encoder = new TextEncoder('charset=utf-8');
    var uint8Array = decoder.decode(new TextEncoder().encode(str));
    return encoder.encode(uint8Array);
}

// 遍历 JSON 对象的属性并转换中文字符的编码格式
function convertJSONToUTF8(obj) {
    for (var key in obj) {
        if (typeof obj[key] === 'string') {
            obj[key] = convertToUTF8(obj[key]);
        } else if (typeof obj[key] === 'object' && obj[key] !== null) {
            convertJSONToUTF8(obj[key]);
        }
    }
    return obj;
}


function loadBlogData() {


    console.log("撒大苏打撒啊飒飒现场支持");

    var xhr = new XMLHttpRequest();
    xhr.open('POST', urlPath + '/blog/src?get=blogData&page=' + currentPage + '&size=' + pageSize, true);

    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {

            console.log(xhr.responseText); ///////////////


            

            var data = JSON.parse(xhr.responseText);

            // var utf8Data = convertJSONToUTF8(data);
            // console.log(utf8Data); ///////////////

            console.log(data); ///////////////

            displayBlogTable(data);
        }
    };
    xhr.send();

    // displayBlogTable(data);

}


function displayBlogTable(data) {

    for (let i = 0; i < data.length; i++) {
        const newDiv = document.createElement('div');
        newDiv.className = 'content-box';
        const paperBox = document.createElement('div');
        paperBox.className = 'paper-box';
        const titleBox = document.createElement('div');
        titleBox.className = 'title-box';
        // titleBox.textContent = `C语言实现BP神经网络${i + 1}`;
        titleBox.textContent = data[i]["btitle"];

        const synopsisBox = document.createElement('div');
        synopsisBox.className = 'synopsis-box';
        // synopsisBox.textContent = `本文简要记录了作者在实现BP神经网络过程，学到的各种知识。注意只是简单记录一下用到的知识点，内容以公式和结论为主。`;
        synopsisBox.textContent = data[i]["bintroduce"];

        const imageBox = document.createElement('div');
        imageBox.className = 'paperimg-box';

        // paperBox.appendChild(titleBox);
        // paperBox.appendChild(synopsisBox);
        // newDiv.appendChild(paperBox);
        // newDiv.appendChild(imageBox); // 将imageBox添加到结构中


        if (i % 2 == 0) {
            paperBox.appendChild(titleBox);
            paperBox.appendChild(synopsisBox);
            newDiv.appendChild(paperBox);
            newDiv.appendChild(imageBox); // 将imageBox添加到结构中
        }
        else {
            newDiv.appendChild(imageBox); // 将imageBox添加到结构中
            paperBox.appendChild(titleBox);
            paperBox.appendChild(synopsisBox);
            newDiv.appendChild(paperBox);

        }

        // 添加点击事件
        newDiv.addEventListener('click', () => {
            window.location.href = urlPath + "/blog/" + data[i]["bpath"]; // 替换为目标URL
        });

        document.getElementById('blogListContainer').appendChild(newDiv);
    }
}





showLeftBox02();
topButtonEvent();
loadBlogData(); 
