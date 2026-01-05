# Node BFF Layer

This directory hosts the optional Node.js BFF that exposes the same `/api` surface as the
frontend, then forwards requests to Spring Boot.

## Responsibilities

- ✅ Request forwarding to Spring Boot (`SPRING_BOOT_URL`, default `http://localhost:8080`).
- ✅ Lightweight parameter validation or response caching when needed.
- ❌ No domain business logic or data ownership. Keep business rules in Spring Boot to avoid
  duplicated responsibilities.

## Example

`GET /api/example?q=keyword` demonstrates query validation and caching, while all other
`/api` paths are proxied directly.
