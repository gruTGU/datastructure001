const cache = new Map();

const DEFAULT_TTL_MS = 30_000;

function getCache(key) {
  const record = cache.get(key);
  if (!record) {
    return null;
  }
  if (Date.now() > record.expiresAt) {
    cache.delete(key);
    return null;
  }
  return record.value;
}

function setCache(key, value, ttlMs = DEFAULT_TTL_MS) {
  cache.set(key, {
    value,
    expiresAt: Date.now() + ttlMs,
  });
}

module.exports = {
  getCache,
  setCache,
};
