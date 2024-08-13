const { logger } = require("firebase-functions");
const { onCall, HttpsError, onRequest } = require("firebase-functions/v2/https");
const { initializeApp } = require('firebase-admin/app');
const { getFirestore, Timestamp, FieldValue } = require('firebase-admin/firestore');
const fetch = require("node-fetch");

initializeApp();
const db = getFirestore();

exports.sendMessage = onCall(
  { region: "asia-southeast2" },
  async (req) => {
    logger.info(req.data);

    if (!req.auth) {
      throw new HttpsError("failed-precondition");
    }

    const response = await fetch(
      "https://uq3tabx7zhy26j25hzdcq6uzd40fsrsx.lambda-url.ap-southeast-1.on.aws/",
      {
        method: "post",
        body: JSON.stringify(req.data),
        headers: { "Content-Type": "application/json" }
      }
    )
    if (!response.ok) {
      throw new Error(`HTTP Error: ${response.status}`);
    }

    return response;
  });

exports.eventUpdate = onRequest(
  {
    region: "asia-southeast2",
    cors: true
  }, async (req, res) => {
    logger.info(req.body);

    const docRef = db.collection("devices").doc(req.body.deviceId);

    docRef.update({
      online: true,
      deviceId: req.body.deviceId,
      temperature: req.body.temperature,
      humidity: req.body.humidity,
      firmwareVersion: req.body.firmwareVersion
    }).then(result => {
      logger.info(`Document updated at ${result.writeTime.toDate()}`);
      res.sendStatus(200);
    }).catch(err => {
      logger.info(`Failed to update document: ${err}`);
      res.sendStatus(500);
    });
  });
