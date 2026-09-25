console.log("Задание 1");
function mergeEventsId(firstSchedule, secondSchedule) {
    const merged = [...firstSchedule, ...secondSchedule];

    return merged.reduce((acc, group) => {
        return acc.concat(group);
    }, []);
}

const firstSchedule = [
    [101, 102],
    [103],
    [104, 105],
];

const secondSchedule = [
    [201],
    [202, 203],
];

console.log(mergeEventsId(firstSchedule, secondSchedule));

console.log("Задание 2");
function sumParticipants(data) {
    return data.reduce((acc, element) => {
        if (Array.isArray(element)) {
            return acc + sumParticipants(element);
        }
        return acc + element;
    }, 0);
}

const participants = [
    12, 
    [5, 8],
    [3, [7, 10]],
    [],
];

console.log(sumParticipants(participants));

console.log("Задание 3");

function groupAvailableEvents(events) {
    return events
        .filter(event => event.participants < event.capacity)
        .reduce((acc, event) => {
            if (!acc[event.category]) {
                acc[event.category] = [];
            }
            acc[event.category].push(event);
            return acc;
        }, {});
}

const events = [
    {
        title: 'Frontend Meetup',
        category: 'education',
        participants: 45,
        capacity: 60,
    },
    {
        title: 'Campus Run',
        category: 'sport',
        participants: 30,
        capacity: 30,
    },
    {
        title: 'JavaScript Workshop',
        category: 'education',
        participants: 18,
        capacity: 25,
    },
    {
        title: 'Music Evening',
        category: 'culture',
        participants: 70,
        capacity: 100,
    },
];

console.log(groupAvailableEvents(events));

console.log("Задание 4");

function analyzeEventTitle(title) {
    const words = title
        .trim()
        .split(' ')
        .filter(word => word.length > 0);

    const wordCount = words.length;
    const longestWord = words.reduce(
        (longest, word) => (word.length > longest.length ? word : longest), ''
    );
    const totalLength = words.reduce((sum, word) => sum + word.length, 0);
    const averageLength = wordCount === 0 ? 0 : totalLength / wordCount;

    return {
        wordCount,
        longestWord,
        averageLength,
    };
}

console.log(analyzeEventTitle('  JavaScript for modern interfaces  '));

console.log("Задание 5");

function mergeSettings(...objects) {
    return Object.assign({}, ...objects);
}

const defaultSettings = {
    theme: 'light',
    language: 'ru',
    pageSize: 10,
};

const userSettings = {
    theme: 'dark',
    pageSize: 20,
};

const sessionSettings = {
    language: 'en',
};

const result = mergeSettings(
    defaultSettings,
    userSettings,
    sessionSettings
);

console.log(result);

console.log("Задание 6");

function createStorageBar(used, limit, length) {
    if (
        typeof used !== 'number' || used < 0 ||
        typeof limit !== 'number' || limit <= 0 ||
        used > limit ||
        !Number.isInteger(length) || length <= 0
    ) {
        return 'Некорректные данные';
    }

    const filled = Math.round((used / limit) * length);
    const empty = length - filled;

    return '#'.repeat(filled) + '-'.repeat(empty);
}

console.log(createStorageBar(20, 100, 10));
console.log(createStorageBar(50, 100, 10));
console.log(createStorageBar(100, 100, 10)); 
console.log(createStorageBar(-1, 100, 10));  
