const ta = document.getElementById("TA");
const contentWrapper = document.getElementById("content-wrapper");
const numReg = /^[0-9]{1,}\. /;
const italicReg = /^\*\S.*\*$/;
const boldReg = /^\*{2}\S.*\*{2}$/;

ta.addEventListener('input', setValue);
ta.addEventListener('keydown', prevTab);

// 解析html字符串
function parse() {
  let htmlStr = ta.value;
  let htmlArr = htmlStr.split('\n');
  let firstMeetLi = true;
  let isOrder = new Array;

  for (let i = 0; i < htmlArr.length; i++) {
    if (htmlArr[i].startsWith('# ')) {
      htmlArr[i] = `<h1>${htmlArr[i].substring(1)}</h1>`;
    }
    else if (htmlArr[i].startsWith('## ')) {
      htmlArr[i] = `<h2>${htmlArr[i].substring(2)}</h2>`;
    }
    else if (htmlArr[i].startsWith('### ')) {
      htmlArr[i] = `<h3>${htmlArr[i].substring(3)}</h3>`;
    }
    else if (htmlArr[i].startsWith('#### ')) {
      htmlArr[i] = `<h4>${htmlArr[i].substring(4)}</h4>`;
    }
    else if (htmlArr[i].startsWith('##### ')) {
      htmlArr[i] = `<h5>${htmlArr[i].substring(5)}</h5>`;
    }
    else if (htmlArr[i].startsWith('###### ')) {
      htmlArr[i] = `<h6>${htmlArr[i].substring(6)}</h6>`;
    }
    else if (htmlArr[i].startsWith('* ') || htmlArr[i].startsWith('- ')) {
      htmlArr[i] = `<li>${htmlArr[i].substring(1)}</li>`;
      isOrder[i] = false;
    }
    else if (htmlArr[i].startsWith('    * ') || htmlArr[i].startsWith('    - ')) {
      htmlArr[i] = `<li class="children">${htmlArr[i].substring(5)}</li>`;
    }
    else if (htmlArr[i].startsWith('        * ') || htmlArr[i].startsWith('       - ')) {
      htmlArr[i] = `<li class="children">${htmlArr[i].substring(9)}</li>`;
    }
    else if (numReg.test(htmlArr[i])) {
      isOrder[i] = true;
      htmlArr[i] = `<li>${htmlArr[i].substring(2)}</li>`;
    }
    else if (htmlArr[i].startsWith('***')) {
      htmlArr[i] = `<hr>`;
    }
    else if (italicReg.test(htmlArr[i]) && !boldReg.test(htmlArr[i])) {
      htmlArr[i] = `<i>${htmlArr[i].substring(1, htmlArr[i].length - 1)}</i>`
    }
    else if (boldReg.test(htmlArr[i])) {
      htmlArr[i] = `<b>${htmlArr[i].substring(2, htmlArr[i].length - 2)}</b>`
    }
    else if (htmlArr[i] === "") {
      htmlArr[i] = '<br/>';
    }
    else {
      htmlArr[i] = `<p>${htmlArr[i]}</p>`;
    }
  }

  // 将li标签放入ul或ol(bug之后改)
  for (let i = 0; i < htmlArr.length; i++) {
    if (htmlArr[i].startsWith('<li')) {
      if (htmlArr[i].startsWith('<li>')) {
        if (isOrder[i] === false) {
          if (firstMeetLi) {
            htmlArr[i] = `<ul>${htmlArr[i]}`;
          }
          if (htmlArr[i + 1] && !htmlArr[i + 1].startsWith('<li')) {
            htmlArr[i] = `${htmlArr[i]}</ul>`;
          }
        }
        else {
          if (firstMeetLi) {
            htmlArr[i] = `<ol>${htmlArr[i]}`;
          }
          if (htmlArr[i + 1] && !htmlArr[i + 1].startsWith('<li')) {
            htmlArr[i] = `${htmlArr[i]}</ol>`;
          }
        }
        firstMeetLi = false;
      }
      // 一级子元素
      else if (htmlArr[i].startsWith('<li class="children">')) {
        if (htmlArr[i - 1].startsWith('<li>')) {
          htmlArr[i] = `<ul>${htmlArr[i]}`;
        }
        if (htmlArr[i + 1] && htmlArr[i + 1].startsWith('<li>')) {
          htmlArr[i] = `${htmlArr[i]}</ul>`;
        }
        else if (i === htmlArr.length - 1) {
          htmlArr[i] = `${htmlArr[i]}</ul>`;
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
  // 处理嵌套的li(bug之后改)
  let ulArr = contentWrapper.getElementsByTagName("ul");
  for (let i = 0; i < ulArr.length; i++) {
    let liArr = ulArr[i].getElementsByTagName("li");
    let lcArr = ulArr[i].getElementsByClassName("children");
    for (let j = 0; j < ulArr.length; j++) {
      if (liArr[j] && liArr[j].className === "") {
        for (let k = 0; k < lcArr.length; k++) {
          liArr[j].appendChild(lcArr[k]);
        }
      }
    }
  }
}

// 更替preview中的内容
function setValue() {
  let htmlStr = parse();
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