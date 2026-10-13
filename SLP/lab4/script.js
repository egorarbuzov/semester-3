console.log("Задание 1");

const categories = new Set();

function addCategory(category) {
  categories.add(category);
}

function removeCategory(category) {
  categories.delete(category);
}

function hasCategory(category) {
  return categories.has(category);
}

function getCategoriesCount() {
  return categories.size;
}

function getCategories() {
  return [...categories].sort();
}

addCategory('Спорт');
addCategory('Образование');
addCategory('Развлечения');
addCategory('Спорт');

console.log(getCategories());
console.log(getCategoriesCount());
console.log(hasCategory('Спорт'));

removeCategory('Спорт');
console.log(getCategories());


console.log("Задание 2");

const events = [
  {
    id: 1,
    title: 'Вечер настольных игр',
    category: 'Развлечения',
    participants: 24,
    isActive: true,
  },
  {
    id: 2,
    title: 'Frontend Meetup',
    category: 'Образование',
    participants: 40,
    isActive: true,
  },
  {
    id: 3,
    title: 'Осенний забег',
    category: 'Спорт',
    participants: 18,
    isActive: false,
  },
];


function findById(id) {
  return events.find(event => event.id === id);
}

function getActiveEvents() {
  return events.filter(event => event.isActive);
}

function getByCategory(category) {
  return events.filter(event => event.category === category);
}

function hasEventsWithoutParticipants() {
  return events.some(event => event.participants === 0);
}

function sortByParticipants() {
  return [...events].sort((a, b) => a.participants - b.participants);
}

function getTitles() {
  return events.map(event => event.title);
}

function getTotalParticipants() {
  return events.reduce((sum, event) => sum + event.participants, 0);
}

console.log(findById(2));
console.log(getActiveEvents());
console.log(getByCategory('Спорт'));
console.log(hasEventsWithoutParticipants());
console.log(sortByParticipants());
console.log(getTitles());
console.log(getTotalParticipants());


console.log("Задание 3");

const registrations = new Map();

registrations.set(1, {
  title: 'Вечер настольных игр',
  participants: 2,
  price: 10,
});

function addRegistration(id, title, participants, price) {
  if (registrations.has(id)) {
    const ex = registrations.get(id);
    ex.participants = ex.participants + participants;
  } else {
    registrations.set(id, {title, participants, price});
  }
}

function hasRegistration(id) {
  return registrations.has(id);
}

function removeRegistration(id) {
  registrations.delete(id);
}

function changeParticipants(id, participants) {
  if (registrations.has(id)) {
    registrations.get(id).participants = participants;
  }
}

function changePrice(id, price) {
  if (registrations.has(id)) {
    registrations.get(id).price = price;
  }
}

function getRegistrationsCount() {
  return registrations.size;
}

function getTotalTickets() {
  let total = 0;
  for (const reg of registrations.values()) {
    total += reg.participants;
  }
  return total;
}

function getTotalPrice() {
  let total = 0;
  for (const reg of registrations.values()) {
    total += reg.participants * reg.price;
  }
  return total;
}

addRegistration(2, 'Frontend Meetup', 3, 15);
addRegistration(1, 'Вечер настольных игр', 4, 10);

console.log(hasRegistration(1));
console.log(getRegistrationsCount());
console.log(getTotalTickets());
console.log(getTotalPrice());

changeParticipants(1, 10);
changePrice(1, 20);
console.log(getTotalTickets());
console.log(getTotalPrice());

removeRegistration(2);
console.log(getRegistrationsCount());


console.log("Задание 4");

const statistics = {
  education: 12,
  sport: 8,
  entertainment: 15,
};

const keys = Object.keys(statistics);
console.log(keys);

const values = Object.values(statistics);
console.log(values);

const entries = Object.entries(statistics);
const lines = entries.map(([key, value]) => `${key}: ${value}`);
console.log(lines);

const total = values.reduce((sum, v) => sum + v, 0);

console.log(total);

const filtered = Object.fromEntries(
  entries.filter(([, value]) => value > 10)
);
console.log(filtered);


console.log("Задание 5");

const cache = new WeakMap();

function getEventStatistics(event) {
  if (cache.has(event)) {
    console.log('Результат получен из кеша');
    return cache.get(event);
  }

  console.log('Выполняется расчёт');

  const maxPlaces = 40;
  const availablePlaces = maxPlaces - event.participants;
  const occupancyPercent = Math.round((event.participants / maxPlaces) * 100);

  const result = { availablePlaces, occupancyPercent };
  cache.set(event, result);
  return result;
}

const event = { id: 1, title: 'Вечер настольных игр', participants: 24 };

getEventStatistics(event);
getEventStatistics(event);
getEventStatistics({ ...event });