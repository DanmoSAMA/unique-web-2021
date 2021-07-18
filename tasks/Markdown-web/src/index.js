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

const titleReg = /^\s{0,3}#{1,6}\s/; const sharpReg = /^\s{0,3}#{1,6}/;
const enterReg = /\n$/; const italicReg = /\*\S.*\*/;
const boldReg = /\*{2}\S.*\*{2}/; const hyperReg = /\[.+\]\(.+\)/;
const liReg = /^[\*\-]\s/; const liChildrenReg = /^\s{4}[\*\-]\s/;
const numReg = /^[0-9]{1,}\. /; const numChildenReg = /^\s{4}[0-9]{1,}\. /;

ta.addEventListener('input', setValue);
ta.addEventListener('keydown', prevTab);
ip.addEventListener('input', () => {
  localStorage.setItem('title', ip.value);
}); // 保存标题
downloadBtn.addEventListener('click', () => {
  download(htmlStr, `${localStorage.getItem('title')}.md`);
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
          htmlArr[i] = `<h${index}>${htmlArr[i]}</h${index}>`;
          htmlArr[i] = htmlArr[i].replace(titleRegArr[index], "");
        }
      })
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
    // 分割线
    else if (htmlArr[i].startsWith('***')) {
      htmlArr[i] = `<hr>`;
    }
    // 斜体、粗体
    else if (italicReg.test(htmlArr[i]) || boldReg.test(htmlArr[i])) {
      const starReg1 = /\*/;
      const starReg2 = /\*{2}/;
      while (starReg2.test(htmlArr[i])) {
        htmlArr[i] = htmlArr[i].replace(/\*{2}/, "<b>");
        htmlArr[i] = htmlArr[i].replace(/\*{2}/, "</b>");
      }
      while (starReg1.test(htmlArr[i])) {
        htmlArr[i] = htmlArr[i].replace(/\*{1}/, "<i>");
        htmlArr[i] = htmlArr[i].replace(/\*{1}/, "</i>");
      }
      htmlArr[i] += '<br>';
    }
    // 超链接
    else if (hyperReg.test(htmlArr[i])) {
      const hyperRegArr = [/\[.+\]/, /\(.+\)/, /\[.+\]\(.+\)/];
      let infoArr = htmlArr[i].match(hyperRegArr[0]);
      let urlArr = htmlArr[i].match(hyperRegArr[1]);

      let infoStr = infoArr[0].replace(/\[/, "").replace(/\]/, "");
      let urlStr = urlArr[0].replace(/\(/, "").replace(/\)/, "");
      htmlArr[i] = htmlArr[i].replace(hyperRegArr[2], `<a href='${urlStr}'>${infoStr}</a>`);
    }
    else if (htmlArr[i] === "") {
      htmlArr[i] = '<br/>';
    }
    else {
      // 不转义时#的解析方式：
      if (sharpReg.test(htmlArr[i])) {
        const sharpRegArr = [/^\s{0,3}#{1,6}[^#]+/, /^\s{0,3}#{7,}/, /#{1,6}/];
        if (!sharpRegArr[0].test(htmlArr[i]) && !sharpRegArr[1].test(htmlArr[i])) {
          htmlArr[i] = htmlArr[i].replace(sharpRegArr[2], "");
        }
      }
      // 转义
      else if (htmlArr[i].startsWith('\\')) {
        htmlArr[i] = htmlArr[i].replace(/\\/, "");
      }
      htmlArr[i] = `<p>${htmlArr[i]}</p>`;
    }
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
      let k = 0;
      for (let j = 0; j < liArr.length; j++) {
        if (liArr[j].className === "father") {
          if (childrenUl[k]) {
            liArr[j].appendChild(childrenUl[k++]);
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

  let start = this.selectionStart;
  let end = this.selectionEnd;

  if (start === end) {
    document.execCommand('insertText', false, "    ");
  }
  else {
    let strBefore = this.value.slice(0, start);
    let curLineStart = strBefore.includes('\n') ? strBefore.lastIndexOf('\n') + 1 : 0;
    let strBetween = this.value.slice(curLineStart, end + 1);
    let newStr = "    " + strBetween.replace(/\n/g, '\n  ');
    let lineBreakCount = strBetween.split('\n').length;
    let newStart = start + 2;
    let newEnd = end + (lineBreakCount + 1) * 2;

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