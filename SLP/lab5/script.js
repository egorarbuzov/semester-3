console.log("Задание 1");
function debounce(callback, delay) {
  let timerId = null;

  return function (...args) {
    if (timerId !== null) {
      clearTimeout(timerId); 
    }

    timerId = setTimeout(() => {
      callback.apply(this, args);
      timerId = null;
    }, delay);
  };
}

function searchEvents(query) {
  console.log(`Выполняется поиск: ${query}`);
}

const debouncedSearch = debounce(searchEvents, 500);

const searchInput = document.querySelector('#search');
searchInput.addEventListener('input', (event) => {
  debouncedSearch(event.target.value);
});

console.log("Задание 2");
function once(callback) {
  let called = false;
  let result;

  return function (...args) {
    if (!called) {
      called = true;
      result = callback.apply(this, args);
    }
    return result;
  };
}

function submitForm() {
  console.log('Форма отправлена');
}

const submitOnce = once(submitForm);
submitOnce();
submitOnce();
submitOnce();

const initialize = once(() => 'Приложение инициализировано');
console.log(initialize());
console.log(initialize());

console.log("Задание 3")
function filterBy(property) {
  return function (value) {
    return function (items) {
      return items.filter((item) => item[property] === value);
    };
  };
}

const events = [
  { id: 1, title: 'JavaScript Meetup', category: 'education', isActive: true },
  { id: 2, title: 'University Run', category: 'sport', isActive: false },
  { id: 3, title: 'React Workshop', category: 'education', isActive: true },
];

const filterByCategory = filterBy('category');
const filterEducationEvents = filterByCategory('education');
console.log(filterEducationEvents(events));

console.log("Задание 4");
function* createBatches(items, batchSize) {
  if (!Number.isInteger(batchSize) || batchSize <= 0) {
    throw new Error('batchSize должен быть положительным целым числом');
  }

  for (let i = 0; i < items.length; i += batchSize) {
    yield items.slice(i, i + batchSize);
  }
}

const users = ['Анна', 'Иван', 'Мария', 'Алексей', 'Ольга', 'Максим', 'Елена'];

const batches = createBatches(users, 3);
let step = 0;

console.log(`next() ${++step}:`, batches.next().value);
console.log(`next() ${++step}:`, batches.next().value);
console.log(`next() ${++step}:`, batches.next().value);
console.log(`next() ${++step}: done =`, batches.next().done);

for (const batch of createBatches(users, 3)) {
  console.log('Обрабатывается порция:', batch);
}

console.log("Задание 5");
function createRegistrationHandler(eventId, callback) {
  return function (event) {
    callback(eventId, event);
  };
}
function registerForEvent(eventId, event) {
  console.log(`Регистрация на мероприятие ${eventId}`);
}

const handleFirstEvent = createRegistrationHandler(101, registerForEvent);
const handleSecondEvent = createRegistrationHandler(205, registerForEvent);

handleFirstEvent();
handleSecondEvent();

const button = document.querySelector('#register-button');
button.addEventListener(
  'click',
  createRegistrationHandler(101, registerForEvent)
);
