// 读取配置文件
$.ajax({
  type: "get",
  url: "../config.json",
  dataType: "json",
  success: function (data) {
    if (data.theme === "dark") {
      document.body.className = "dark";
    }
    if (data['font-size'] === "small") {
      document.getElementById("editor-mid-layer").className = "small";
      document.getElementById("content-wrapper").className = "small";
    }
    else if (data['font-size'] === "large") {
      document.getElementById("editor-mid-layer").className = "large";
      document.getElementById("content-wrapper").className = "large";
    }
  },
  error: function (err) {
    console.error(err);
  }
});

const ta = document.getElementById("TA");
const ip = document.getElementById("IP");
const downloadBtn = document.getElementById("download");
const contentWrapper = document.getElementById("content-wrapper");

const transReg = /\\\\/g;
const titleReg = /^\s{0,3}#{1,6}\s/;
const sharpReg = /^\s{0,3}#{1,6}/;
const transSharpReg = /\\(#+\s*)+/; // bug
const enterReg = /\n$/;
const italicReg = /\*\S.*\*/;
const boldReg = /\*{2}\S.*\*{2}/;
const hyperReg = /\[.+\]\(.+\)/g;
const liReg = /^[\*\-]\s/;
const liChildrenReg = /^\s{4}[\*\-]\s/;
const numReg = /^[0-9]{1,}\. /;
const numChildenReg = /^\s{4}[0-9]{1,}\. /;
const deviderReg = /^\s{0,3}[\*\-]{1,}\s{0,}[\*\-]{1,}\s{0,}[\*\-]{1,}\s{0,}/;
const bracketRegArr = [/\(/g, /\)/g, /\\\(/g, /\\\)/g, /\[/g, /\]/g, /\\\[/g, /\\\]/g]; //

ta.addEventListener('input', setValue);
ta.addEventListener('keydown', prevTab);
ip.addEventListener('input', () => {
  localStorage.setItem('title', ip.value);
}); // 保存标题

downloadBtn.addEventListener('click', () => {
  let currentTitle = localStorage.getItem('title');
  if (currentTitle !== "") {
    download(htmlStr, `${currentTitle}.md`);
  }
  else {
    download(htmlStr, `default.md`);
  }
}); // 下载

// 读取localStorage
let htmlStr = localStorage.getItem('htmlStr');
if (htmlStr !== null) {
  ta.value = htmlStr;
  parseEl();
  setValue();
}

// 读取title
let title = localStorage.getItem('title');
ip.value = title;

// 解析html字符串
function parseEl() {
  let htmlStr = ta.value;
  let htmlArr = htmlStr.split('\n');
  let firstMeetLi = true;
  let isOrder = new Array;

  localStorage.setItem('htmlStr', htmlStr);

  for (let i = 0; i < htmlArr.length; i++) {
    // 标题
    if (titleReg.test(htmlArr[i])) {
      let titleRegArr = new Array;
      for (let i = 1; i <= 6; i++) {
        titleRegArr[i] = new RegExp(`^\\s{0,3}#{${i}}\\s`);
      }
      titleRegArr.forEach((item, index) => {
        if (titleRegArr[index].test(htmlArr[i])) {
          htmlArr[i] = htmlArr[i].replace(titleRegArr[index], "");
          htmlArr[i] = `<h${index}>${htmlArr[i]}</h${index}>`;
        }
      })
    }
    // 分割线
    else if (deviderReg.test(htmlArr[i])) {
      htmlArr[i] = `<hr>`;
    }
    // 无序列表
    else if (liReg.test(htmlArr[i]) || liChildrenReg.test(htmlArr[i])) {
      if (liReg.test(htmlArr[i])) {
        htmlArr[i] = htmlArr[i].replace(liReg, "");
        htmlArr[i] = `<li>${htmlArr[i]}</li>`;
      }
      else {
        htmlArr[i] = htmlArr[i].replace(liChildrenReg, "");
        htmlArr[i] = `<li class="children">${htmlArr[i]}</li>`;
      }
      isOrder[i] = false;
    }
    // 有序列表
    else if (numReg.test(htmlArr[i]) || numChildenReg.test(htmlArr[i])) {
      if (numReg.test(htmlArr[i])) {
        htmlArr[i] = htmlArr[i].replace(numReg, "");
        htmlArr[i] = `<li>${htmlArr[i]}</li>`;
      }
      else {
        htmlArr[i] = htmlArr[i].replace(numChildenReg, "");
        htmlArr[i] = `<li class="children">${htmlArr[i]}</li>`;
      }
      isOrder[i] = true;
    }
    // 斜体、粗体
    else if (italicReg.test(htmlArr[i]) || boldReg.test(htmlArr[i])) {
      const transStarRegArr = [/\\\*/, /\\\*\*/];
      const starRegArr = [/\*/, /\*{2}/];
      // bug
      while (transStarRegArr[0].test(htmlArr[i])) {
        htmlArr[i] = htmlArr[i].replace(transStarRegArr[0], "");
      }
      while (transStarRegArr[1].test(htmlArr[i])) {
        htmlArr[i] = htmlArr[i].replace(transStarRegArr[1], "");
      }
      while (starRegArr[1].test(htmlArr[i])) {
        htmlArr[i] = htmlArr[i].replace(starRegArr[1], "<b>");
        htmlArr[i] = htmlArr[i].replace(starRegArr[1], "</b>");
      }
      while (starRegArr[0].test(htmlArr[i])) {
        htmlArr[i] = htmlArr[i].replace(starRegArr[0], "<i>");
        htmlArr[i] = htmlArr[i].replace(starRegArr[0], "</i>");
      }
      htmlArr[i] += '<br>';
    }
    // 超链接
    else if (hyperReg.test(htmlArr[i])) {
      const hyperRegArr = [/\[[^\[^\]]+\]/g, /\([^\[^\)]+\)/g, /\[[^\[]+\]\([^\)]+\)/];
      let infoArr = htmlArr[i].match(hyperRegArr[0]);
      let urlArr = htmlArr[i].match(hyperRegArr[1]);
      let infoStrArr = new Array;
      let urlStrArr = new Array;

      infoArr.forEach((info, i) => {
        infoStrArr[i] = info.replace(/\[/, "").replace(/\]/, "");
      })
      urlArr.forEach((url, i) => {
        urlStrArr[i] = url.replace(/\(/, "").replace(/\)/, "");
      })
      infoStrArr.forEach((info, index) => {
        htmlArr[i] = htmlArr[i].replace(hyperRegArr[2], `<a href='${urlStrArr[index]}'>${infoStrArr[index]}</a>`);
      })
      htmlArr[i] = `${htmlArr[i]}<br>`
    }
    else if (htmlArr[i] === "") {
      htmlArr[i] = '<br/>';
    }
    else {
      // 井号
      if (sharpReg.test(htmlArr[i])) {
        const sharpRegArr = [/^\s{0,3}#{1,6}[^#]+/, /^\s{0,3}#{7,}/, /#{1,6}/];
        if (!sharpRegArr[0].test(htmlArr[i]) && !sharpRegArr[1].test(htmlArr[i])) {
          htmlArr[i] = htmlArr[i].replace(sharpRegArr[2], "");
        }
      }
      // 标题转义
      else if (transSharpReg.test(htmlArr[i])) {
        // bug
        console.log(htmlArr[i]);
        htmlArr[i] = htmlArr[i].replace(transSharpReg, "#");
      }
      htmlArr[i] = `<p>${htmlArr[i]}</p>`;
    }
    // 转义
    htmlArr[i] = htmlArr[i].replace(bracketRegArr[2], "(");
    htmlArr[i] = htmlArr[i].replace(bracketRegArr[3], ")");
    htmlArr[i] = htmlArr[i].replace(bracketRegArr[6], "[");
    htmlArr[i] = htmlArr[i].replace(bracketRegArr[7], "]");
    htmlArr[i] = htmlArr[i].replace(transReg, "\\");
  }

  // 将li标签放入ul或ol
  for (let i = 0; i < htmlArr.length; i++) {
    if (htmlArr[i].startsWith('<li')) {
      if (htmlArr[i].startsWith('<li>')) {
        if (!isOrder[i]) {
          // 第一个父li
          if (firstMeetLi) {
            htmlArr[i] = `<ul class="fatherUl">${htmlArr[i]}`;
          }
          // 最后一个父li
          if (!htmlArr[i + 1] || !htmlArr[i + 1].startsWith('<li')) {
            htmlArr[i] = `${htmlArr[i]}</ul>`;
          }
        }
        else {
          if (firstMeetLi) {
            htmlArr[i] = `<ol class="fatherOl">${htmlArr[i]}`;
          }
          if (htmlArr[i + 1] && !htmlArr[i + 1].startsWith('<li')) {
            htmlArr[i] = `${htmlArr[i]}</ol>`;
          }
        }
        firstMeetLi = false;
      }
      else if (htmlArr[i].startsWith('<li class="children">')) {
        if (!isOrder[i]) {
          // 第一个子li
          if (htmlArr[i - 1].startsWith('<ul class="fatherUl"><li>')) {
            htmlArr[i - 1] = htmlArr[i - 1].replace(`<ul class="fatherUl"><li>`, `<ul class="fatherUl"><li class="father">`);
            htmlArr[i] = `<ul class="childrenUl">${htmlArr[i]}`;
          }
          if (htmlArr[i - 1].startsWith('<li>')) {
            htmlArr[i - 1] = htmlArr[i - 1].replace(`<li>`, `<li class="father">`);
            htmlArr[i] = `<ul class="childrenUl">${htmlArr[i]}`;
          }
          // 最后一个子li，但下边还有父li
          if (htmlArr[i + 1] && htmlArr[i + 1].startsWith('<li>')) {
            htmlArr[i] = `${htmlArr[i]}</ul>`;
          }
          // 最后一个子li，也是最后一个li
          if (!htmlArr[i + 1] || !htmlArr[i + 1].startsWith('<li')) {
            htmlArr[i] = `${htmlArr[i]}</ul></ul>`;
          }
        }
        else {
          if (htmlArr[i - 1].startsWith('<ol class="fatherOl"><li>')) {
            htmlArr[i - 1] = htmlArr[i - 1].replace(`<ol class="fatherOl"><li>`, `<ol class="fatherOl"><li class="father">`);
            htmlArr[i] = `<ol class="childrenOl">${htmlArr[i]}`;
          }
          if (htmlArr[i - 1].startsWith('<li>')) {
            htmlArr[i - 1] = htmlArr[i - 1].replace(`<li>`, `<li class="father">`);
            htmlArr[i] = `<ol class="childrenOl">${htmlArr[i]}`;
          }
          if (htmlArr[i + 1] && htmlArr[i + 1].startsWith('<li>')) {
            htmlArr[i] = `${htmlArr[i]}</ol>`;
          }
          if (!htmlArr[i + 1] || !htmlArr[i + 1].startsWith('<li')) {
            htmlArr[i] = `${htmlArr[i]}</ol></ol>`;
          }
        }
      }
    }
    else {
      firstMeetLi = true;
    }
  }
  let newHtmlStr = htmlArr.join('\n');
  return newHtmlStr;
}

function dealChildren() {
  // 处理嵌套的li
  let dealObj = new Object;
  dealObj.fatherUl = contentWrapper.getElementsByTagName("ul");
  dealObj.fatherOl = contentWrapper.getElementsByTagName("ol");
  for (key in dealObj) {
    for (let i = 0; i < dealObj[key].length; i++) {
      let liArr = dealObj[key][i].getElementsByTagName("li");
      let childrenUl = dealObj[key][i].getElementsByClassName("childrenUl");
      let childrenOl = dealObj[key][i].getElementsByClassName("childrenOl");
      let k = 0, l = 0;
      for (let j = 0; j < liArr.length; j++) {
        if (liArr[j].className === "father") {
          if (childrenUl[k]) {
            liArr[j].appendChild(childrenUl[k++]);
          }
          if (childrenOl[l]) {
            liArr[j].appendChild(childrenOl[l++]);
          }
        }
      }
    }
  }
}

// 更替preview中的内容
function setValue() {
  contentWrapper.innerHTML = parseEl();
  dealChildren();
}

// 阻止触发tab的默认事件
function prevTab(event) {
  if (event.code !== "Tab") return true;

  event.preventDefault();

  // 元素中当前选中文本的起始和结束位置
  let start = this.selectionStart;
  let end = this.selectionEnd;

  // 当前未选中文本，直接插入空格
  if (start === end) {
    document.execCommand('insertText', false, "    ");
  }
  // '123456789'选中34，按下tab
  else {
    let strBefore = this.value.slice(0, start);
    console.log(strBefore);       // '12'
    let curLineStart = strBefore.includes('\n') ? strBefore.lastIndexOf('\n') + 1 : 0;
    console.log(curLineStart);    // 0
    let strBetween = this.value.slice(curLineStart, end + 1);
    console.log(strBetween);      // '12345'
    let newStr = "    " + strBetween.replace(/\n/g, '\n  ');
    console.log(newStr);          // '    12345'
    let lineBreakCount = strBetween.split('\n').length;
    console.log(lineBreakCount);  // 1
    let newStart = start + 2;
    console.log(newStart);        // 4
    let newEnd = end + (lineBreakCount + 1) * 2;
    console.log(newEnd);          // 8

    this.setSelectionRange(curLineStart, end);
    document.execCommand("insertText", false, newStr);
    this.setSelectionRange(newStart, newEnd);
  }
}

// 下载
function download(content, filename) {
  // 创建隐藏的可下载链接
  let eleLink = document.createElement('a');
  // 设置download属性，属性值为文件名
  eleLink.download = filename;
  // eleLink.style.display = 'none';

  // 字符内容转变成blob地址
  let blob = new Blob([content]);
  // 得到URL对象，赋给href属性
  eleLink.href = URL.createObjectURL(blob);
  // document.body.appendChild(eleLink);

  // 模拟a标签触发点击事件，与download属性配合完成下载
  eleLink.click();
  // document.body.removeChild(eleLink);
};