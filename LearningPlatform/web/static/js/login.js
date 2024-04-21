$(document).ready(function() {
    $('button').click(function() {
      // 获取输入的账号和密码
      let username = $('input[type="text"]').val();
      let password = $('input[type="password"]').val();
  
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
  
      // 发送POST请求
      $.ajax({
        url: urlPath + '/login/src',
        type: 'POST',
        contentType: 'application/json',
        data: JSON.stringify(data),
          success: function (response) {

            console.log(response);

            if (response.islogin === "true") {   // {"islogin": "true"} 注意响应报文中的Content-Type也要设置为json
                window.location.replace(urlPath + "/main");
            } else {
                alert('账号或密码错误');
            }
        },
        error: function(xhr, status, error) {
          alert('发生错误：' + error);
        }
      });
    });
  });