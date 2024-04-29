
document.addEventListener("DOMContentLoaded", function() {
  document.getElementById('btn1').addEventListener('click', function() {
      // 获取输入的账号和密码
      let username = document.querySelector('input[type="text"]').value;
      let password = document.querySelector('input[type="password"]').value;  
      // 检查账号和密码格式
      let usernameRegex = /^[0-9a-zA-Z]+$/;
      if (!usernameRegex.test(username)) {
          alert('账号格式不正确，请使用0~9和a~z的字符。');
          return;
      }
      if (!usernameRegex.test(password)) {
          alert('密码格式不正确，请使用0~9和a~z的字符。');
          return;
      } 
      // 创建包含账号和密码的JSON对象
      let data = {
          uaccount: username,
          upassword: password
      };  
      // 创建 XMLHttpRequest 对象
      let xhr = new XMLHttpRequest(); 
      // 设置请求类型、URL和异步标志
      xhr.open('POST', urlPath + '/login/src?action=login', true); 
      // 设置请求头
      xhr.setRequestHeader('Content-Type', 'application/json'); 
      // 处理响应
      xhr.onreadystatechange = function() {
          if (xhr.readyState === XMLHttpRequest.DONE) {
              if (xhr.status === 200) {
                  let response = JSON.parse(xhr.responseText);
                  console.log(response);
                  if (response.islogin === "true") {
                      window.location.replace(urlPath + "/main");
                  } else {
                      alert('账号或密码错误');
                  }
              } else {
                  alert('发生错误：' + xhr.statusText);
              }
          }
      };  
      // 发送请求
      xhr.send(JSON.stringify(data));
  });

  document.getElementById('btn2').addEventListener('click', function() {
    // 获取输入的账号和密码
    let username = document.querySelector('input[type="text"]').value;
    let password = document.querySelector('input[type="password"]').value;  
    // 检查账号和密码格式
    let usernameRegex = /^[0-9a-zA-Z]+$/;
    if (!usernameRegex.test(username)) {
        alert('账号格式不正确，请使用0~9和a~z的字符。');
        return;
    }
    if (!usernameRegex.test(password)) {
        alert('密码格式不正确，请使用0~9和a~z的字符。');
        return;
    } 
    // 创建包含账号和密码的JSON对象
    let data = {
        uaccount: username,
        upassword: password
    };  
    // 创建 XMLHttpRequest 对象
    let xhr = new XMLHttpRequest(); 
    // 设置请求类型、URL和异步标志
    xhr.open('POST', urlPath + '/login/src?action=enroll', true); 
    // 设置请求头
    xhr.setRequestHeader('Content-Type', 'application/json'); 
    // 处理响应
    xhr.onreadystatechange = function() {
        if (xhr.readyState === XMLHttpRequest.DONE) {
            if (xhr.status === 200) {
                let response = JSON.parse(xhr.responseText);
                console.log(response);
                if (response.islogin === "true") {
                    window.location.replace(urlPath + "/main");
                } else {
                    alert('账号不能重复');
                }
            } else {
                alert('发生错误：' + xhr.statusText);
            }
        }
    };  
    // 发送请求
    xhr.send(JSON.stringify(data));
});

});