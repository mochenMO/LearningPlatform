// var submitA = document.getElementById("btna");

// submitA.addEventListener('click', ()=> {
//   // history.back();   // 不是返回上一个页面，而是返回主页面 ！！！！！！

//   let username = getCookie("username");

//   if(username == null || username == "nulluser"){
//     window.location.replace("http://localhost:8888/login/login");
//   }
//   else{
//     window.location.replace("http://localhost:8888/index/index");
//   }
// });


// function getCookie(name) {
// var cookies = document.cookie.split(';');
// for (var i = 0; i < cookies.length; i++) {
// var cookie = cookies[i];
// // 去除左侧空格
// while (cookie.charAt(0) == ' ') {
//     cookie = cookie.substring(1);
// }
// // 匹配cookie名称
// if (cookie.indexOf(name) == 0) {
//     return decodeURIComponent(cookie.substring(name.length + 1));
// }
// }
// // 没有找到对应的cookie，返回null
// return null;
// }


document.getElementById("btna").addEventListener('click', ()=> {
  window.history.back();
});