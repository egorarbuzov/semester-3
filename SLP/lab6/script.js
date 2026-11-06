console.log("Задание 1");

const coordinates = [53.902284, 27.561831, 215];

const [latitude, longitude, altitude = 0] = coordinates;

console.log("Широта:", latitude);
console.log("Долгота:", longitude);
console.log("Высота:", altitude);

function showLocation(latitude, longitude, altitude) {
  console.log(`Координаты: ${latitude}, ${longitude}. Высота: ${altitude}`);
}

showLocation(...coordinates);

console.log("Задание 2");

const user = {
  id: 17,
  firstName: 'Анна',
  lastName: 'Ковалева',
  email: 'anna@example.com',
  role: 'student',
  isActive: true,
};

const {
  id: userId,
  firstName: name,
  lastName: surname,
  avatar = 'default-avatar.png',
  ...userDetails
} = user;

console.log("userId:", userId);
console.log("name:", name);
console.log("surname:", surname);
console.log("avatar:", avatar);
console.log("userDetails:", userDetails);

const admin = {
  ...user,
  role: 'admin',
  permissions: ['read', 'create', 'edit'],
};

console.log("admin:", admin);
console.log("user не изменился:", user);

console.log("Задание 3");

const event = {
  id: 101,
  title: 'Frontend Meetup',
  category: 'education',
  location: {
    building: {
      name: 'Главный корпус',
      address: {
        city: 'Минск',
        street: 'ул. Свердлова',
        buildingNumber: '13а',
      },
    },
    room: {
      number: '305',
      floor: 3,
    },
  },
  schedule: {
    date: '2026-10-15',
    time: {
      start: '18:00',
      end: '20:30',
    },
  },
  organizer: {
    profile: {
      firstName: 'Мария',
      lastName: 'Соколова',
      contacts: {
        email: 'maria@example.com',
        phone: '+375291234567',
      },
    },
  },
  participants: [
    { id: 1, name: 'Анна', status: 'confirmed' },
    { id: 2, name: 'Иван', status: 'waiting' },
    { id: 3, name: 'Ольга', status: 'confirmed' },
  ],
};

const {
  id: eventId,
  title: eventTitle,
  location: {
    building: {
      name: buildingName,
      address: { city, street, buildingNumber },
    },
    room: { number: roomNumber, floor },
  },
  schedule: {
    date,
    time: { start, end },
  },
  organizer: {
    profile: {
      firstName: organizerFirstName,
      lastName: organizerLastName,
      contacts: { email: organizerEmail },
    },
  },
  participants,
} = event;

console.log("eventId:", eventId);
console.log("eventTitle:", eventTitle);
console.log("buildingName:", buildingName);
console.log("city:", city, "street:", street, "buildingNumber:", buildingNumber);
console.log("roomNumber:", roomNumber, "floor:", floor);
console.log("date:", date);
console.log("start:", start, "end:", end);
console.log("organizer:", organizerFirstName, organizerLastName, organizerEmail);
console.log("participants:", participants);

const [yearStr, monthStr, dayStr] = date.split('-');
const formattedDate = `${dayStr}.${monthStr}.${yearStr}`;

const eventDescription =
  `${eventTitle} пройдёт ${formattedDate} с ${start} до ${end} по адресу: ` +
  `${city}, ${street}, ${buildingNumber}, ${buildingName}, аудитория ${roomNumber}. ` +
  `Организатор: ${organizerFirstName} ${organizerLastName}.`;

console.log(eventDescription);

const confirmedNames = participants
  .filter(({ status }) => status === 'confirmed')
  .map(({ name }) => name);

console.log("confirmedNames:", confirmedNames);

console.log("Задание 4");

const profile = {
  id: 7,
  name: 'Ирина',
  settings: {
    theme: 'light',
    language: 'ru',
    notifications: {
      email: true,
      push: true,
      sms: false,
    },
  },
};

const updatedProfile = {
  ...profile,
  name: 'Ирина Петрова',
  settings: {
    ...profile.settings,
    theme: 'dark',
    notifications: {
      ...profile.settings.notifications,
      push: false,
    },
  },
};

console.log("updatedProfile:", updatedProfile);
console.log("profile не изменился:", profile);

console.log("Задание 5");

const events = [
  { id: 1, title: 'JavaScript Meetup', category: 'education', participants: 35 },
  { id: 2, title: 'University Run', category: 'sport', participants: 48 },
];

const newEvent = {
  id: 3,
  title: 'Design Workshop',
  category: 'education',
  participants: 20,
};

const eventsWithNewEvent = [...events, newEvent];
console.log("eventsWithNewEvent:", eventsWithNewEvent);

function updateEvent(events, eventId, changes) {
  return events.map((event) =>
    event.id === eventId ? { ...event, ...changes } : event
  );
}

const updatedEvents = updateEvent(eventsWithNewEvent, 2, {
  title: 'Autumn University Run',
  participants: 55,
});

console.log("updatedEvents:", updatedEvents);
console.log("исходный массив не изменился:", eventsWithNewEvent);

console.log("Задание 6");

function calculateAverage(...values) {
  if (values.length === 0) {
    return 'Некорректные данные';
  }

  if (!values.every((value) => typeof value === 'number' && !Number.isNaN(value))) {
    return 'Некорректные данные';
  }

  const sum = values.reduce((acc, value) => acc + value, 0);
  return sum / values.length;
}

console.log(calculateAverage(8, 9, 10));
console.log(calculateAverage(5, 7, 8, 10));
console.log(calculateAverage());
console.log(calculateAverage(5, '7', 8));

const marks = [5, 7, 8, 10];
console.log("Среднее из массива:", calculateAverage(...marks));

console.log("Задание 7");

const data = {
  project: {
    info: {
      name: 'Digital Space',
      version: '2.4.0',
      authors: [
        {
          id: 1,
          profile: {
            name: 'Анна',
            contacts: {
              email: 'anna@example.com',
              social: { telegram: '@anna_dev' },
            },
          },
        },
        {
          id: 2,
          profile: {
            name: 'Иван',
            contacts: {
              email: 'ivan@example.com',
              social: { telegram: '@ivan_dev' },
            },
          },
        },
      ],
    },
    settings: {
      interface: {
        language: 'ru',
        theme: {
          name: 'dark',
          colors: { primary: '#6c5ce7' },
        },
      },
      features: {
        registration: true,
        notifications: true,
        analytics: false,
      },
    },
    statistics: {
      users: {
        total: 1250,
        activity: { today: 145, week: 684, month: 1027 },
      },
    },
  },
};

const {
  project: {
    info: {
      name: projectName,
      version: projectVersion,
      authors: [firstAuthor, ...otherAuthors],
    },
    settings: {
      interface: {
        language: interfaceLanguage,
        theme: {
          name: themeName,
          colors: { primary: primaryColor, secondary: secondaryColor = '#ffffff' },
        },
      },
      features: { registration, notifications },
    },
    statistics: {
      users: {
        total: totalUsers,
        activity: { today, week, month },
      },
    },
  },
} = data;

const {
  profile: {
    name: firstAuthorName,
    contacts: {
      email: firstAuthorEmail,
      social: { telegram: firstAuthorTelegram },
    },
  },
} = firstAuthor;

console.log("projectName:", projectName);
console.log("projectVersion:", projectVersion);
console.log("firstAuthor:", firstAuthor);
console.log("otherAuthors:", otherAuthors);
console.log("firstAuthorName:", firstAuthorName);
console.log("firstAuthorEmail:", firstAuthorEmail);
console.log("firstAuthorTelegram:", firstAuthorTelegram);
console.log("interfaceLanguage:", interfaceLanguage);
console.log("themeName:", themeName);
console.log("primaryColor:", primaryColor);
console.log("secondaryColor:", secondaryColor);
console.log("registration:", registration);
console.log("notifications:", notifications);
console.log("totalUsers:", totalUsers);
console.log("today:", today, "week:", week, "month:", month);