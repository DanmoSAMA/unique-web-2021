// 使用ajax读取配置文件
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

const titleReg = /^\s{0,3}#{1,6}\s/;
const sharpReg = /^\s{0,3}#{1,6}/;
const enterReg = /\n$/;
const italicReg = /\*\S.*\*/;
const boldReg = /\*{2}\S.*\*{2}/;
const hyperReg = /\[.+\]\(.+\)/;

const liReg = /^[\*\-]\s/;
const liChildrenReg = /^\s{4}[\*\-]\s/;
const numReg = /^[0-9]{1,}\. /;
const numChildenReg = /^\s{4}[0-9]{1,}\. /;

ta.addEventListener('input', setValue);
ta.addEventListener('keydown', prevTab);
ip.addEventListener('input', saveTitle);

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

//下载
downloadBtn.addEventListener('click', () => {
  download(htmlStr, `${title}.md`);    
});

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
      const titleReg2 = /^\s{0,3}#{2}\s/; const titleReg3 = /^\s{0,3}#{3}\s/;
      const titleReg4 = /^\s{0,3}#{4}\s/; const titleReg5 = /^\s{0,3}#{5}\s/;
      const titleReg6 = /^\s{0,3}#{6}\s/;

      if (titleReg2.test(htmlArr[i])) {
        htmlArr[i] = `<h2>${htmlArr[i]}</h2>`;
        htmlArr[i] = htmlArr[i].replace(/##/, "");
      }
      else if (titleReg3.test(htmlArr[i])) {
        htmlArr[i] = `<h3>${htmlArr[i]}</h3>`;
        htmlArr[i] = htmlArr[i].replace(/###/, "");
      }
      else if (titleReg4.test(htmlArr[i])) {
        htmlArr[i] = `<h4>${htmlArr[i]}</h4>`;
        htmlArr[i] = htmlArr[i].replace(/####/, "");
      }
      else if (titleReg5.test(htmlArr[i])) {
        htmlArr[i] = `<h5>${htmlArr[i]}</h5>`;
        htmlArr[i] = htmlArr[i].replace(/#####/, "");
      }
      else if (titleReg6.test(htmlArr[i])) {
        htmlArr[i] = `<h6>${htmlArr[i]}</h6>`;
        htmlArr[i] = htmlArr[i].replace(/######/, "");
      }
      else {
        htmlArr[i] = `<h1>${htmlArr[i]}</h1>`;
        htmlArr[i] = htmlArr[i].replace(/#/, "");
      }
    }
    // 无序列表
    else if (liReg.test(htmlArr[i])) {
      htmlArr[i] = htmlArr[i].replace(liReg, "");
      htmlArr[i] = `<li>${htmlArr[i]}</li>`;
      isOrder[i] = false;
    }
    else if (liChildrenReg.test(htmlArr[i])) {
      htmlArr[i] = htmlArr[i].replace(liChildrenReg, "");
      htmlArr[i] = `<li class="children">${htmlArr[i]}</li>`;
      isOrder[i] = false;
    }
    // 有序列表
    else if (numReg.test(htmlArr[i])) {
      htmlArr[i] = htmlArr[i].replace(numReg, "");
      htmlArr[i] = `<li>${htmlArr[i]}</li>`;
      isOrder[i] = true;
    }
    else if (numChildenReg.test(htmlArr[i])) {
      htmlArr[i] = htmlArr[i].replace(numChildenReg, "");
      htmlArr[i] = `<li class="children">${htmlArr[i]}</li>`;
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
      const hyperReg1 = /\[.+\]/;
      const hyperReg2 = /\(.+\)/;
      let infoArr = htmlArr[i].match(hyperReg1);
      let urlArr = htmlArr[i].match(hyperReg2);

      let infoStr = infoArr[0].replace(/\[/, "").replace(/\]/, "");
      let urlStr = urlArr[0].replace(/\(/, "").replace(/\)/, "");

      htmlArr[i] = htmlArr[i].replace(/\[.+\]\(.+\)/, `<a href='${urlStr}'>${infoStr}</a>`);
    }
    else if (htmlArr[i] === "") {
      htmlArr[i] = '<br/>';
    }
    else {
      // 没有遇到\时，#的解析方式：
      if (sharpReg.test(htmlArr[i])) {
        const sharpReg1 = /^\s{0,3}#{1,6}[^#]+/;
        const sharpReg2 = /^\s{0,3}#{7,}/;
        if (!sharpReg1.test(htmlArr[i]) && !sharpReg2.test(htmlArr[i])) {
          htmlArr[i] = htmlArr[i].replace(/#{1,6}/, "");
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
      // 父li
      if (htmlArr[i].startsWith('<li>')) {
        // 无序列表
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
        // 有序列表
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
      // 子li
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
          // 最后一个子li，下边还有父li
          if (htmlArr[i + 1] && htmlArr[i + 1].startsWith('<li>')) {
            htmlArr[i] = `${htmlArr[i]}</ul>`;
          }
          // 最后一个子li，也是最后一个li
          else if (!htmlArr[i + 1] || !htmlArr[i + 1].startsWith('<li')) {
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
          else if (!htmlArr[i + 1] || !htmlArr[i + 1].startsWith('<li')) {
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
  const fatherUl = contentWrapper.getElementsByTagName("ul");
  const fatherOl = contentWrapper.getElementsByTagName("ol");
  for (let i = 0; i < fatherUl.length; i++) {
    let liArr = fatherUl[i].getElementsByTagName("li");
    let childrenUl = fatherUl[i].getElementsByClassName("childrenUl");
    let k = 0;
    for (let j = 0; j < liArr.length; j++) {
      if (liArr[j].className === "father") {
        if (childrenUl[k]) {
          liArr[j].appendChild(childrenUl[k++]);
        }
      }
    }
  }
  for (let i = 0; i < fatherOl.length; i++) {
    let liArr = fatherOl[i].getElementsByTagName("li");
    let childrenOl = fatherOl[i].getElementsByClassName("childrenOl");
    let k = 0;
    for (let j = 0; j < liArr.length; j++) {
      if (liArr[j].className === "father") {
        if (childrenOl[k]) {
          liArr[j].appendChild(childrenOl[k++]);
        }
      }
    }
  }
}

// 更替preview中的内容
function setValue() {
  let htmlStr = parseEl();
  contentWrapper.innerHTML = htmlStr;
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

// 存储title
function saveTitle() {
  localStorage.setItem('title', ip.value);
}

// 下载，明天理解此代码
function download(content, filename) {
  var eleLink = document.createElement('a');
  eleLink.download = filename;
  eleLink.style.display = 'none';
  var blob = new Blob([content]);
  eleLink.href = URL.createObjectURL(blob);
  document.body.appendChild(eleLink);
  eleLink.click();
  document.body.removeChild(eleLink);
};