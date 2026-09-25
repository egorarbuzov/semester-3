console.log("Задание 1");
function basicOperation(operation, value1, value2) {
    if (operation === '+') { return value1 + value2; }
    else if (operation === '-') { return value1 - value2; }
    else if (operation === '*') { return value1 * value2; }
    else if (operation === '/') { return value1 / value2; }
    else { return undefined; }
}   

console.log(basicOperation('+', 4, 5));
console.log(basicOperation('/', 10, 2));

console.log("Задание 2");
function sumOfCubes(n) {
    let sum = 0;
    for (let i = 1; i <= n; i++) {
        sum += i ** 3;
    }
    return sum;
}

console.log(sumOfCubes(12));

console.log("Задание 3");
function averageSumOfArr(arr) {
    let sumArr = 0;
    for (let i = 0; i < arr.length; i++) {
        sumArr +=arr[i];
    }
    return sumArr / arr.length;
}

let arr = [1, 2, 3, 4, 5];
console.log(averageSumOfArr(arr));

console.log("Задание 4");
function reverseAndFilter(str) {
    let result = '';
    
    for (let i = str.length - 1; i >= 0; i--) {
        const char = str[i];
        if ((char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z')) {
            result += char;
        }
    }
    
    return result;
}

console.log("Задание 5");
console.log(reverseAndFilter("he3llo87427"));
console.log(reverseAndFilter("JavaScr53э? ipt"))

function printStringNtimes(str, amount) {
    let newStr = "";
    for (let i = 0; i < amount; i++) {
        newStr +=str;
    }
    return newStr;
}

console.log(printStringNtimes("hello", 5));


console.log("Задание 6");
function difference(arr1, arr2) {
    return arr1.filter(str => !arr2.includes(str));
}
 
const arr1 = ["apple", "banana", "cherry", "date"];
const arr2 = ["banana", "date", "fig"];
console.log(difference(arr1, arr2));