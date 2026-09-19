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
    
}

const Event = {
    title: string,
    category: string,
    participants: number,
    capacity: number
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

