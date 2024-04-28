
var currentPage = 1; // 当前页码
var pageSize = 10; // 每页显示的记录数
var isLastPage = false;
var loadDataFunction;
var displayDataFunction;


// 初始化菜单并显示第一个菜单的内容
document.addEventListener('DOMContentLoaded', function() {
    // 初始化时显示用户信息表单

    // console.log("123");////////////////

    loadUserData();

    // 绑定菜单项点击事件
    document.getElementById('userMenu').addEventListener('click', loadUserData);
    document.getElementById('blogMenu').addEventListener('click', loadBlogData);
});



// ==========================================================================================================================================
// 显示用户信息表单

function loadUserData() {

    loadDataFunction = loadUserData;
    displayDataFunction = displayUserTable;

    var xhr = new XMLHttpRequest();
    xhr.open('POST', urlPath + '/backend/src?get=userData&page=' + currentPage + '&size=' + pageSize, true);

    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {

            console.log(xhr.responseText); ///////////////

            var data = JSON.parse(xhr.responseText);

            // var utf8Data = convertJSONToUTF8(data);
            // console.log(utf8Data); ///////////////

            console.log(data); ///////////////

            displayUserTable(data);
        }
    };
    xhr.send();
}



// 显示用户数据表格
function displayUserTable(datas) {

    if (datas.length < pageSize) {
        isLastPage = true;
    } else {
        isLastPage = false;
    }

    // var userContent = document.getElementById('userContent');
    var userContent = document.getElementById('tableContainer');////////////

    userContent.innerHTML = ''; // 清空之前的内容

    // 创建表格
    var table = document.createElement('table');
    var thead = document.createElement('thead');
    var tbody = document.createElement('tbody');


    var tr = document.createElement('tr');
    var th1 = document.createElement('th');
    th1.textContent = '用户ID';

    var th2 = document.createElement('th');
    th2.textContent = '用户类型';

    var th3 = document.createElement('th');
    th3.textContent = '账号';

    var th4 = document.createElement('th');
    th4.textContent = '用户密码';

    var th5 = document.createElement('th');
    th5.textContent = '用户手机号';

    var th6 = document.createElement('th');
    th6.textContent = '用户邮箱号';

    var th7 = document.createElement('th');
    th7.textContent = '用户年龄';

    var th8 = document.createElement('th');
    th8.textContent = '操作';

    tr.appendChild(th1);
    tr.appendChild(th2);
    tr.appendChild(th3);
    tr.appendChild(th4);
    tr.appendChild(th5);
    tr.appendChild(th6);
    tr.appendChild(th7);
    tr.appendChild(th8);
    thead.appendChild(tr);
    table.appendChild(thead);


    // 表格内容
    datas.forEach(function(data) {
        var tr = document.createElement('tr');
        var td1 = document.createElement('td');
        td1.textContent = data.uid;

        var td2 = document.createElement('td');
        td2.textContent = data.utype;

        var td3 = document.createElement('td');
        td3.textContent = data.uaccount;

        var td4 = document.createElement('td');
        td4.textContent = data.upassword;

        var td5 = document.createElement('td');
        td5.textContent = data.uphone;

        var td6 = document.createElement('td');
        td6.textContent = data.uemail;

        var td7 = document.createElement('td');
        td7.textContent = data.uage;

        var td8 = document.createElement('td');
        var editBtn = document.createElement('button');
        editBtn.textContent = '修改';
        editBtn.onclick = function() {
            editUser(data);
        };
        var deleteBtn = document.createElement('button');
        deleteBtn.textContent = '删除';
        deleteBtn.onclick = function() {
            deleteUser(data.uid);
        };
        td8.appendChild(editBtn);
        td8.appendChild(deleteBtn);

        tr.appendChild(td1);
        tr.appendChild(td2);
        tr.appendChild(td3);
        tr.appendChild(td4);
        tr.appendChild(td5);
        tr.appendChild(td6);
        tr.appendChild(td7);
        tr.appendChild(td8);
        tbody.appendChild(tr);
    });
    table.appendChild(tbody);
    userContent.appendChild(table);
}



// 搜索
function searchData() {
  
    // 获取输入框元素
    var searchInput = document.getElementById('searchInput');
    // 获取输入框内容
    var searchText = searchInput.value;
    // 发送搜索请求
    var xhr = new XMLHttpRequest();
    xhr.open('POST', urlPath + '/backend/src?search=userData&tag=' + searchText, true);

    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {

            console.log(xhr.responseText); ///////////////
            var data = JSON.parse(xhr.responseText);
            console.log(data); ///////////////
            displayDataFunction(data);
        }
    };
    xhr.send();
}


// 添加
function addData(){}


// 上一页
function prevPage()
{
    console.log("prevPage"); ///////////
    if (currentPage > 1) {
        currentPage--;
        document.getElementById("pageNumber").textContent = currentPage;
        loadDataFunction();
    }
}

// 下一页
function nextPage()
{
    console.log("nextPage");//////////////
    if (isLastPage == false) {
        currentPage++;
        document.getElementById("pageNumber").textContent = currentPage;
        loadDataFunction();
    }
}



function editUser(data) {
    

    var modal = document.getElementById("userModal");
    // var openModalBtn = document.getElementById("openModalBtn");
    var closeModalBtn = document.querySelector(".close");
    var confirmBtn = document.getElementById("confirmBtn");
    var cancelBtn = document.getElementById("cancelBtn");
    

    var uidInput = document.getElementById("uid");
    var utypeInput = document.getElementById("utype");
    var uaccountInput = document.getElementById("uaccount");
    var upasswordInput = document.getElementById("upassword");
    var uphoneInput = document.getElementById("uphone");
    var uemailInput = document.getElementById("uemail");
    var uageInput = document.getElementById("uage");


    uidInput.value = data.uid;
    utypeInput.value = data.utype;
    uaccountInput.value = data.uaccount;
    upasswordInput.value = data.upassword;
    uphoneInput.value = data.uphone;
    uemailInput.value = data.uemail;
    uageInput.value = data.uage;


    modal.style.display = "block";
      
    closeModalBtn.onclick = function() {
        modal.style.display = "none";
    }

    confirmBtn.onclick = function() {
        modal.style.display = "none";

        
        // 设置默认值
        var userInfo = {
          uid: "null",
          utype: "null",
          uaccount: "null",
          upassword: "null",
          uphone: "null",
          uemail: "null",
          uage: "null"
        };
    
        if (uidInput.value != "") {
            userInfo["uid"] = uidInput.value;
        }
        if (utypeInput.value != "") {
            userInfo["utype"] = utypeInput.value;
        }
        if (uaccountInput.value != "") {
            userInfo["uaccount"] = uaccountInput.value;
        }
        if (upasswordInput.value != "") {
            userInfo["upassword"] = upasswordInput.value;
        }
        if (uphoneInput.value != "") {
            userInfo["uphone"] = uphoneInput.value;
        }
        if (uemailInput.value != "") {
            userInfo["uemail"] = uemailInput.value;
        }
        if (uageInput.value != "") {
            userInfo["uage"] = uageInput.value;
        }
        
        // 创建 JSON 对象
        var jsonData = JSON.stringify(userInfo);
    
        // 发送 JSON 数据到服务器
        var xhr = new XMLHttpRequest();
        xhr.open('POST', urlPath + '/backend/src?update=userData', true);
        xhr.setRequestHeader("Content-Type", "application/json");
        xhr.onreadystatechange = function() {
          if (xhr.readyState === 4 && xhr.status === 200) {
            // 请求成功，可以处理返回的数据
            console.log("Data sent successfully!");
            loadDataFunction();
          }
        };
        xhr.send(jsonData);

    }
    
    cancelBtn.onclick = function() {
        modal.style.display = "none";
    }
}

// 删除用户
function deleteUser(userId) {
    
    if (window.confirm("确定删除该数据吗？") == true) {
        
        var temp ={ uid: userId};

        // 创建 JSON 对象
        var jsonData = JSON.stringify(temp);
    
        // 发送 JSON 数据到服务器
        var xhr = new XMLHttpRequest();
        xhr.open('POST', urlPath + '/backend/src?delete=userData', true);
        xhr.setRequestHeader("Content-Type", "application/json");
        xhr.onreadystatechange = function() {
           if (xhr.readyState === 4 && xhr.status === 200) {
             // 请求成功，可以处理返回的数据
             console.log("Data sent successfully!");
             loadDataFunction();
           }
        };
        xhr.send(jsonData);
    }



}


// ==========================================================================================================================================
// 显示blog信息表单

function loadBlogData() {

    loadDataFunction = loadBlogData;
    displayDataFunction = displayBlogData;


    var xhr = new XMLHttpRequest();
    xhr.open('POST', urlPath + '/backend/src?get=blogData&page=' + currentPage + '&size=' + pageSize, true);

    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {

            console.log(xhr.responseText); ///////////////

            var data = JSON.parse(xhr.responseText);

            // var utf8Data = convertJSONToUTF8(data);
            // console.log(utf8Data); ///////////////

            console.log(data); ///////////////

            displayUserTable(data);
        }
    };
    xhr.send();
}


function displayBlogData() {}




// function loadUserData() {

//     loadDataFunction = loadUserData;
//     displayDataFunction = displayUserTable;

//     var xhr = new XMLHttpRequest();
//     xhr.open('POST', urlPath + '/backend/src?get=userData&page=' + currentPage + '&size=' + pageSize, true);

//     xhr.onreadystatechange = function () {
//         if (xhr.readyState == 4 && xhr.status == 200) {

//             console.log(xhr.responseText); ///////////////

//             var data = JSON.parse(xhr.responseText);

//             // var utf8Data = convertJSONToUTF8(data);
//             // console.log(utf8Data); ///////////////

//             console.log(data); ///////////////

//             displayUserTable(data);
//         }
//     };
//     xhr.send();
// }



// // 显示用户数据表格
// function displayUserTable(datas) {

//     if (datas.length < pageSize) {
//         isLastPage = true;
//     } else {
//         isLastPage = false;
//     }

//     // var userContent = document.getElementById('userContent');
//     var userContent = document.getElementById('tableContainer');////////////

//     userContent.innerHTML = ''; // 清空之前的内容

//     // 创建表格
//     var table = document.createElement('table');
//     var thead = document.createElement('thead');
//     var tbody = document.createElement('tbody');


//     var tr = document.createElement('tr');
//     var th1 = document.createElement('th');
//     th1.textContent = '用户ID';

//     var th2 = document.createElement('th');
//     th2.textContent = '用户类型';

//     var th3 = document.createElement('th');
//     th3.textContent = '账号';

//     var th4 = document.createElement('th');
//     th4.textContent = '用户密码';

//     var th5 = document.createElement('th');
//     th5.textContent = '用户手机号';

//     var th6 = document.createElement('th');
//     th6.textContent = '用户邮箱号';

//     var th7 = document.createElement('th');
//     th7.textContent = '用户年龄';

//     var th8 = document.createElement('th');
//     th8.textContent = '操作';

//     tr.appendChild(th1);
//     tr.appendChild(th2);
//     tr.appendChild(th3);
//     tr.appendChild(th4);
//     tr.appendChild(th5);
//     tr.appendChild(th6);
//     tr.appendChild(th7);
//     tr.appendChild(th8);
//     thead.appendChild(tr);
//     table.appendChild(thead);


//     // 表格内容
//     datas.forEach(function(data) {
//         var tr = document.createElement('tr');
//         var td1 = document.createElement('td');
//         td1.textContent = data.uid;

//         var td2 = document.createElement('td');
//         td2.textContent = data.utype;

//         var td3 = document.createElement('td');
//         td3.textContent = data.uaccount;

//         var td4 = document.createElement('td');
//         td4.textContent = data.upassword;

//         var td5 = document.createElement('td');
//         td5.textContent = data.uphone;

//         var td6 = document.createElement('td');
//         td6.textContent = data.uemail;

//         var td7 = document.createElement('td');
//         td7.textContent = data.uage;

//         var td8 = document.createElement('td');
//         var editBtn = document.createElement('button');
//         editBtn.textContent = '修改';
//         editBtn.onclick = function() {
//             editUser(data);
//         };
//         var deleteBtn = document.createElement('button');
//         deleteBtn.textContent = '删除';
//         deleteBtn.onclick = function() {
//             deleteUser(data.uid);
//         };
//         td8.appendChild(editBtn);
//         td8.appendChild(deleteBtn);

//         tr.appendChild(td1);
//         tr.appendChild(td2);
//         tr.appendChild(td3);
//         tr.appendChild(td4);
//         tr.appendChild(td5);
//         tr.appendChild(td6);
//         tr.appendChild(td7);
//         tr.appendChild(td8);
//         tbody.appendChild(tr);
//     });
//     table.appendChild(tbody);
//     userContent.appendChild(table);
// }



// // 搜索
// function searchData() {
  
//     // 获取输入框元素
//     var searchInput = document.getElementById('searchInput');
//     // 获取输入框内容
//     var searchText = searchInput.value;
//     // 发送搜索请求
//     var xhr = new XMLHttpRequest();
//     xhr.open('POST', urlPath + '/backend/src?search=userData&tag=' + searchText, true);

//     xhr.onreadystatechange = function () {
//         if (xhr.readyState == 4 && xhr.status == 200) {

//             console.log(xhr.responseText); ///////////////
//             var data = JSON.parse(xhr.responseText);
//             console.log(data); ///////////////
//             displayDataFunction(data);
//         }
//     };
//     xhr.send();
// }


// // 添加
// function addData(){}


// // 上一页
// function prevPage()
// {
//     console.log("prevPage"); ///////////
//     if (currentPage > 1) {
//         currentPage--;
//         document.getElementById("pageNumber").textContent = currentPage;
//         loadDataFunction();
//     }
// }

// // 下一页
// function nextPage()
// {
//     console.log("nextPage");//////////////
//     if (isLastPage == false) {
//         currentPage++;
//         document.getElementById("pageNumber").textContent = currentPage;
//         loadDataFunction();
//     }
// }



// function editUser(data) {
    

//     var modal = document.getElementById("userModal");
//     // var openModalBtn = document.getElementById("openModalBtn");
//     var closeModalBtn = document.querySelector(".close");
//     var confirmBtn = document.getElementById("confirmBtn");
//     var cancelBtn = document.getElementById("cancelBtn");
    

//     var uidInput = document.getElementById("uid");
//     var utypeInput = document.getElementById("utype");
//     var uaccountInput = document.getElementById("uaccount");
//     var upasswordInput = document.getElementById("upassword");
//     var uphoneInput = document.getElementById("uphone");
//     var uemailInput = document.getElementById("uemail");
//     var uageInput = document.getElementById("uage");


//     uidInput.value = data.uid;
//     utypeInput.value = data.utype;
//     uaccountInput.value = data.uaccount;
//     upasswordInput.value = data.upassword;
//     uphoneInput.value = data.uphone;
//     uemailInput.value = data.uemail;
//     uageInput.value = data.uage;


//     modal.style.display = "block";
      
//     closeModalBtn.onclick = function() {
//         modal.style.display = "none";
//     }

//     confirmBtn.onclick = function() {
//         modal.style.display = "none";

        
//         // 设置默认值
//         var userInfo = {
//           uid: "null",
//           utype: "null",
//           uaccount: "null",
//           upassword: "null",
//           uphone: "null",
//           uemail: "null",
//           uage: "null"
//         };
    
//         if (uidInput.value != "") {
//             userInfo["uid"] = uidInput.value;
//         }
//         if (utypeInput.value != "") {
//             userInfo["utype"] = utypeInput.value;
//         }
//         if (uaccountInput.value != "") {
//             userInfo["uaccount"] = uaccountInput.value;
//         }
//         if (upasswordInput.value != "") {
//             userInfo["upassword"] = upasswordInput.value;
//         }
//         if (uphoneInput.value != "") {
//             userInfo["uphone"] = uphoneInput.value;
//         }
//         if (uemailInput.value != "") {
//             userInfo["uemail"] = uemailInput.value;
//         }
//         if (uageInput.value != "") {
//             userInfo["uage"] = uageInput.value;
//         }
        
//         // 创建 JSON 对象
//         var jsonData = JSON.stringify(userInfo);
    
//         // 发送 JSON 数据到服务器
//         var xhr = new XMLHttpRequest();
//         xhr.open('POST', urlPath + '/backend/src?update=userData', true);
//         xhr.setRequestHeader("Content-Type", "application/json");
//         xhr.onreadystatechange = function() {
//           if (xhr.readyState === 4 && xhr.status === 200) {
//             // 请求成功，可以处理返回的数据
//             console.log("Data sent successfully!");
//             loadDataFunction();
//           }
//         };
//         xhr.send(jsonData);

//     }
    
//     cancelBtn.onclick = function() {
//         modal.style.display = "none";
//     }
// }

// // 删除用户
// function deleteUser(userId) {
    
//     if (window.confirm("确定删除该数据吗？") == true) {
        
//         var temp ={ uid: userId};

//         // 创建 JSON 对象
//         var jsonData = JSON.stringify(temp);
    
//         // 发送 JSON 数据到服务器
//         var xhr = new XMLHttpRequest();
//         xhr.open('POST', urlPath + '/backend/src?delete=userData', true);
//         xhr.setRequestHeader("Content-Type", "application/json");
//         xhr.onreadystatechange = function() {
//            if (xhr.readyState === 4 && xhr.status === 200) {
//              // 请求成功，可以处理返回的数据
//              console.log("Data sent successfully!");
//              loadDataFunction();
//            }
//         };
//         xhr.send(jsonData);
//     }



// }