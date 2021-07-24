const items = document.getElementById("heroes-carousel-items");
const itemArr = [...document.getElementsByClassName("heroes-carousel-item")];
const leftBtn = document.getElementsByClassName("heroes-carousel-icon")[0];
const rightBtn = document.getElementsByClassName("heroes-carousel-icon")[1];

const allBtn = document.getElementById("all-btn");
const tankBtn = document.getElementById("tank-btn");
const damageBtn = document.getElementById("damage-btn");
const supportBtn = document.getElementById("support-btn");
const typeBtnArr = [allBtn, tankBtn, damageBtn, supportBtn];

let length = itemArr.length;
let width = 940;
changeWidth();

leftBtn.addEventListener("click", toLeft);
rightBtn.addEventListener("click", toRight);

window.addEventListener("resize", changeWidth);
allBtn.addEventListener("click", all);
tankBtn.addEventListener("click", tank);
damageBtn.addEventListener("click", damage);
supportBtn.addEventListener("click", support);

items.style.left = 0;

function changeWidth() {
  items.style.left = 0;
  leftBtn.className = "heroes-carousel-icon disabled";
  rightBtn.className = "heroes-carousel-icon";
  let pageWidth = document.documentElement.clientWidth + 17;
  if (pageWidth > 1279) {
    width = 940;
  } else if (pageWidth <= 1279 && pageWidth > 959) {
    width = 770;
  }
}

function toLeft() {
  rightBtn.className = "heroes-carousel-icon";
  if (items.style.left === "-widthpx") {
    leftBtn.className = "heroes-carousel-icon disabled";
  }
  if (items.style.left !== 0 && items.style.left !== "0px") {
    items.style.left = parseInt(items.style.left) + width + "px";
  }
}

function toRight() {
  leftBtn.className = "heroes-carousel-icon";
  if (items.style.left === `${-((Math.ceil(length / 4) - 2) * width)}px`) {
    rightBtn.className = "heroes-carousel-icon disabled";
  }
  if (items.style.left !== `${-((Math.ceil(length / 4) - 1) * width)}px`) {
    items.style.left = parseInt(items.style.left) - width + "px";
  }
}

function all() {
  length = 0;
  items.style.left = 0;
  typeBtnArr.forEach((item) => {
    item.className = "heroes-roleselect-listItem";
  });
  allBtn.className = "heroes-roleselect-listItem active";

  itemArr.forEach((item) => {
    item.style.display = "block";
    length++;
  });
}

function tank() {
  length = 0;
  items.style.left = 0;
  typeBtnArr.forEach((item) => {
    item.className = "heroes-roleselect-listItem";
  });
  tankBtn.className = "heroes-roleselect-listItem active";

  itemArr.forEach((item) => {
    if (/tank/.test(item.className)) {
      item.style.display = "block";
      length++;
    } else {
      item.style.display = "none";
    }
  });
}

function damage() {
  length = 0;
  items.style.left = 0;
  typeBtnArr.forEach((item) => {
    item.className = "heroes-roleselect-listItem";
  });
  damageBtn.className = "heroes-roleselect-listItem active";

  itemArr.forEach((item) => {
    if (/damage/.test(item.className)) {
      item.style.display = "block";
      length++;
    } else {
      item.style.display = "none";
    }
  });
}

function support() {
  length = 0;
  items.style.left = 0;
  typeBtnArr.forEach((item) => {
    item.className = "heroes-roleselect-listItem";
  });
  supportBtn.className = "heroes-roleselect-listItem active";

  itemArr.forEach((item) => {
    if (/support/.test(item.className)) {
      item.style.display = "block";
      length++;
    } else {
      item.style.display = "none";
    }
  });
}
