import http from 'k6/http';
import { check } from 'k6';

export const options = {
  stages: [
    { duration: '5s', target: 500 },
    { duration: '10s', target: 2000 },
    { duration: '5s', target: 0 },
  ],
};

export default function () {
  const res = http.get('http://127.0.0.1:8080/json');
  check(res, {
    'status is 200': (r) => r.status === 200,
    'content-type is json': (r) => r.headers['Content-Type'] === 'application/json',
  });
}
