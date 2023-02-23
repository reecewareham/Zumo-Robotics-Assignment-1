float wrapAngle(float angle) {
  angle = fmod(angle + 180, 360);
  if (angle < 0) {
    angle += 360;
  }
  return (angle - 180);
}

bool inRange(float value, float lowerBound, float upperBound) {
  return ((value > lowerBound) && (value < upperBound));
}