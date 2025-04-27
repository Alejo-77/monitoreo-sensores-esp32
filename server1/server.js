const express = require('express');
const mongoose = require('mongoose');
const cors = require('cors');

// Conexión a MongoDB Atlas (reemplaza la URL)
mongoose.connect('mongodb+srv://ah773080:<db_password>@cluster0.z9gurx2.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0')
  .then(() => console.log("Conectado a MongoDB"))
  .catch(err => console.error("Error:", err));

// Esquema de datos
const datoSchema = new mongoose.Schema({
  luz: Number,
  temp: Number,
  humedad: Number,
  movimiento: Boolean,
  fecha: { type: Date, default: Date.now }
});
const Dato = mongoose.model('Dato', datoSchema);

// Configuración del servidor
const app = express();
app.use(cors());
app.use(express.json());

// Ruta para recibir datos del ESP32
app.post('/api/data', async (req, res) => {
  try {
    const { luz, temp, humedad, movimiento } = req.body;
    const nuevoDato = new Dato({ luz, temp, humedad, movimiento });
    await nuevoDato.save();
    res.status(200).json({ mensaje: "Datos guardados." });
  } catch (error) {
    res.status(500).json({ error: "Error al guardar datos." });
  }
});

// Iniciar servidor
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Servidor en http://localhost:${PORT}`);
});
