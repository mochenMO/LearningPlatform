let userData;


function getUserData()
{
    console.log("getUserData");

    // 向服务器请求用户的信息
    var xhr = new XMLHttpRequest();
    xhr.open("GET", urlPath + "/main/src?get=userData", true);
    xhr.responseType = 'json';
    xhr.send();

    // 请求完成时的处理函数(异步运行)
    xhr.onload = function() {
        if (xhr.status == 200) {
            // let userInfo = xhr.response;
            // window.userInfo = userInfo;
            // console.log("userData:", userInfo);
            // console.log("Global userData:", window.userInfo);
            userData = xhr.response;
            console.log("userData:", userData); ///////////////////////////
            if(userData != null){
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

    if(userData != null) {
        document.querySelector('.username-input-box:nth-of-type(1) input').placeholder = userData["uaccount"];
        document.querySelector('.username-input-box:nth-of-type(2) input').placeholder = userData["upassword"];
        document.querySelector('.username-input-box:nth-of-type(3) input').placeholder = userData["uphone"];
        document.querySelector('.username-input-box:nth-of-type(4) input').placeholder = userData["uemail"];
        document.querySelector('.username-input-box:nth-of-type(5) input').placeholder = userData["uage"];
    }
   


    document.querySelector('.headshot-box').addEventListener('click', function() {
        console.log("back22 ok");
       
    });

    document.querySelector('.btn1-box').addEventListener('click', function() {
        // 获取所有输入框的值

       if (window.confirm("确认要修改吗？") == true){

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
        
        
            if (uaccountInput.value != ""){
                userInfo["uaccount"] = uaccountInput.value;
                uaccountInput.placeholder = uaccountInput.value;
            }
            if (upasswordInput.value != ""){
                userInfo["upassword"] = upasswordInput.value;
                upasswordInput.placeholder = upasswordInput.value;
            }
            if (uphoneInput.value != ""){
                userInfo["uphone"] = uphoneInput.value;
                uphoneInput.placeholder = uphoneInput.value;
            }
            if (uemailInput.value != ""){
                userInfo["uemail"] = uemailInput.value;
                uemailInput.placeholder = uemailInput.value;
            }
            if (uageInput.value != ""){
                userInfo["uage"] = uageInput.value;
                uageInput.placeholder = uageInput.value;
            }
        
            userData = userInfo;
            
        
            // 将 JSON 对象发送给服务器
            var jsonData = JSON.stringify(userInfo);
            var xhr = new XMLHttpRequest();
            xhr.open('POST', urlPath + '/main/src?save=userData', true);
            xhr.setRequestHeader('Content-Type', 'application/json');
        
            xhr.onreadystatechange = function() {
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

    document.querySelector('.btn2-box').addEventListener('click', function() {
        if (window.confirm("确定退出登录吗？") == true){
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

    document.querySelector('.btn3-box').addEventListener('click', function() {
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


    document.querySelector('.headshot-box').addEventListener('click', function() {
        showLeftBox01();
    });

    document.querySelector('.btn1-box').addEventListener('click', function() {
        console.log("showLeftBox02 btn1_boxbtn1_box ok");
    });



    if(userData == null){
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
function topButtonEvent()
{
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

// 置顶功能






showLeftBox02();
topButtonEvent();


