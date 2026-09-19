console.log("Задание 1");
let a = 5;
debugger;
let name = "Name";
let i = 0;
let double = 0.23;
let result = 1 / 0;
let answer = true;
let no = null;
console.log(typeof(a));
console.log(typeof(name));
console.log(typeof(i));
console.log(typeof(double));
console.log(typeof(result));
console.log(typeof(answer));
console.log(typeof(no));

console.log("Задание 2");
let squareSide = 5;
let rectWidth = 45;
let rectHeight = 21;
let countByWidth = Math.floor(rectWidth / squareSide);
let countByHeight = Math.floor(rectHeight / squareSide);
let total = countByWidth * countByHeight;
console.log(`Ответ: ${total} квадратов`);

console.log("Задание 3");
let i_2 = 2;
let a_2 = ++i;
let b_2 = i++;
let res_2 = a_2 > b_2;
console.log(res_2);
debugger;

console.log("Задание 4");
console.log("Котик" === "котик" ? "Равны" : "Не равны");
console.log("Объяснение: Строки чувствительны к регистру\n");

console.log("Котик" === "китик" ? "Равны" : "Не равны");
console.log("Объяснение: Разные символы ('о' vs 'и')\n");

console.log("Кот" === "Котик" ? "Равны" : "Не равны");
console.log("Объяснение: Разная длина строк\n");

console.log("Привет" === "Пока" ? "Равны" : "Не равны");
console.log("Объяснение: Совершенно разные строки\n");

console.log(73 == "53" ? "Равны" : "Не равны");
console.log("Объяснение: '53' → 53, 73 ≠ 53\n");

console.log(false == 0 ? "Равны" : "Не равны");
console.log("Объяснение: false → 0, поэтому равны\n");

console.log(54 == true ? "Равны" : "Не равны");
console.log("Объяснение: true → 1, 54 ≠ 1\n");

console.log(123 == false ? "Равны" : "Не равны");
console.log("Объяснение: false → 0, 123 ≠ 0\n");

console.log(true == "3" ? "Равны" : "Не равны");
console.log("Объяснение: true → 1, '3' → 3, 1 ≠ 3\n");

console.log(3 == "5мм" ? "Равны" : "Не равны");
console.log("Объяснение: '5мм' → NaN, NaN ≠ 3\n");

console.log(8 == "-2" ? "Равны" : "Не равны");
console.log("Объяснение: '-2' → -2, 8 ≠ -2\n");

console.log(34 == "34" ? "Равны" : "Не равны");
console.log("Объяснение: '34' → 34, поэтому равны\n");

console.log(null == undefined ? "Равны" : "Не равны");
console.log("Объяснение: По правилам JavaScript null == undefined\n");

const teacherName = "Андрей";
let userInput = prompt("Введите ФИО преподавателя: ", "");
if (userInput === null || userInput.trim() === "") {
    alert("Вы отменили ввод или ничего не ввели.");
} else {
    let normalizedInput = userInput.trim().toLowerCase();
    let normalizedTeacher = teacherName.toLowerCase();
    let words = normalizedInput.split(/\s+/);
    let isValid = words.includes(normalizedTeacher);

    if (isValid === true) {
        alert("Введенные данные верные!");
    } else {
        alert("Введенные данные НЕверные!.");
    }
}

let russian = confirm("Сдал ли студент русский? (ок - да, отмена - нет)");
let math = confirm("Сдал ли студент математику? (ок - да, отмена - нет)");
let english = confirm("Сдал ли студент английский? (ок - да, отмена - нет)");

if (russian && math && english) {
    alert("Студент переведен на следующий курс!");
} else if (!russian && !math && !english) {
    alert("Студент отчислен!");
} else {
    alert("Студента ожидает пересдача!");
}

console.log("Задание 7");
console.log("1. true + true =", true + true);
console.log("Объяснение: true преобразуется в 1, 1 + 1 = 2\n");

console.log("2. 0 + \"5\" =", 0 + "5");
console.log("Объяснение: Число 0 преобразуется в строку \"0\", конкатенация строк: \"0\" + \"5\" = \"05\"\n");

console.log("3. 5 + \"MM\" =", 5 + "MM");
console.log("Объяснение: Число 5 преобразуется в строку \"5\", конкатенация: \"5\" + \"MM\" = \"5MM\"\n");

console.log("4. 8/Infinity =", 8/Infinity);
console.log("Объяснение: Деление на бесконечность дает 0\n");

console.log("5. 9 * \"\\n9\" =", 9 * "\n9");
console.log("Объяснение: Строка \"\\n9\" преобразуется в число 9 (символ новой строки игнорируется), 9 * 9 = 81\n");

console.log("6. null - 1 =", null - 1);
console.log("Объяснение: null преобразуется в 0, 0 - 1 = -1\n");

console.log("7. \"5\" - 2 =", "5" - 2);
console.log("Объяснение: Строка \"5\" преобразуется в число 5, 5 - 2 = 3\n");

console.log("8. \"5px\" - 3 =", "5px" - 3);
console.log("Объяснение: Строка \"5px\" не может быть преобразована в число → NaN, NaN - 3 = NaN\n");

console.log("9. true - 3 =", true - 3);
console.log("Объяснение: true преобразуется в 1, 1 - 3 = -2\n");

console.log("10. 7 || 0 =", 7 || 0);
console.log("Объяснение: Оператор || возвращает первое истинное значение. 7 - истина, поэтому возвращает 7\n");

console.log("Задание 8");

for (let i = 1; i <= 10; i++) {
    if (i % 2 === 0) {
        console.log(i + 2);
    } else {
        console.log(i + "мм");
    }
}

console.log("Задание 9");
const week = {
    1: "пн",
    2: "вт",
    3: "ср",
    4: "чт",
    5: "пт",
    6: "сб",
    7: "вс",
};
let chosenDay = prompt("ОБЪЕКТ: Введите номер дня недели (1-7): ", "");
if (chosenDay === null || chosenDay.trim() === "") {
    alert("Вы отменили ввод или ничего не ввели.");
} else {
    let num = Number(chosenDay);
    if (num >= 1 && num <= 7) {
        let dayName = week[num];
        alert(`${num} - это ${dayName}`);
        console.log(`${num} → ${dayName}`);
    } else {
        alert("Ошибка: введите число от 1 до 7");
        console.log(`Некорректный ввод: ${chosenDay}`);
    }
}

const daysArr = ["вс", "пн", "вт", "ср", "чт", "пт", "сб", "вс"];

let num = Number(prompt("МАССИВ: Введите номер дня (1-7):", ""));

if (num >= 1 && num <= 7) {
    alert(`${num} - это ${daysArr[num]}`);
    console.log(`${num} → ${daysArr[num]}`);
} else {
    alert("Ошибка: введите число от 1 до 7");
}

console.log("Задание 10");
function createString(param1 = "Значение по умолчанию", param2, param3) {
    return `${param1} ${param2} ${param3}`;
}

let userParam = prompt("Введите третий параметр:", "");

if (userParam === null || userParam.trim() === "") {
    alert("Вы отменили ввод или ничего не ввели.");
} else {
    let result = createString(undefined, "Второй параметр", userParam);
    alert(`Результат: "${result}"`);
}

console.log("Задание 11");
function paramsDeclaration(a, b) {
    if (a === b) {
        return 4 * a;
    } else {
        return a * b;
    }
}

const paramsExpression = function(a, b) {
    if (a === b) {
        return 4 * a;
    } else {
        return a * b;
    }
};

const paramsArrow = (a, b) => {
    if (a === b) {
        return 4 * a;
    } else {
        return a * b;
    }
};

let a_sqr = 5;
let b_sqr = 5;

console.log(`Квадрат (${a_sqr}, ${b_sqr}):`);
console.log(`Declaration: ${paramsDeclaration(a_sqr, b_sqr)}`);
console.log(`Expression:  ${paramsExpression(a_sqr, b_sqr)}`);
console.log(`Arrow:       ${paramsArrow(a_sqr, b_sqr)}`);

let a_rect = 5;
let b_rect = 8;

console.log(`\nПрямоугольник (${a_rect}, ${b_rect}):`);
console.log(`Declaration: ${paramsDeclaration(a_rect, b_rect)}`);
console.log(`Expression:  ${paramsExpression(a_rect, b_rect)}`);
console.log(`Arrow:       ${paramsArrow(a_rect, b_rect)}`);