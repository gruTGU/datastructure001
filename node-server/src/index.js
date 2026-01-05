const express = require("express");
const { createProxyMiddleware } = require("http-proxy-middleware");
const { z } = require("zod");
const { getCache, setCache } = require("./simpleCache");

const app = express();

const springBootUrl = process.env.SPRING_BOOT_URL || "http://localhost:8080";
const port = process.env.PORT || 3000;

const exampleQuerySchema = z.object({
  q: z.string().min(1, "q is required").max(100),
});

app.get("/api/example", async (req, res) => {
  const parseResult = exampleQuerySchema.safeParse(req.query);
  if (!parseResult.success) {
    return res.status(400).json({
      message: "Invalid query parameters",
      issues: parseResult.error.flatten(),
    });
  }

  const cacheKey = `example:${req.originalUrl}`;
  const cached = getCache(cacheKey);
  if (cached) {
    return res.status(200).json({
      source: "cache",
      data: cached,
    });
  }

  const upstreamUrl = new URL(req.originalUrl, springBootUrl);
  const response = await fetch(upstreamUrl, {
    headers: {
      accept: req.header("accept") || "application/json",
    },
  });

  const data = await response.json();
  setCache(cacheKey, data);

  return res.status(response.status).json({
    source: "spring-boot",
    data,
  });
});

app.use(
  "/api",
  createProxyMiddleware({
    target: springBootUrl,
    changeOrigin: true,
    secure: false,
    logLevel: "warn",
  })
);

app.listen(port, () => {
  // eslint-disable-next-line no-console
  console.log(`BFF listening on http://localhost:${port}`);
});
