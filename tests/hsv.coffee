gtk = require '../build/default/gtk'

process.nextTick ->
  gtk.init()

  r = new gtk.Entry
  g = new gtk.Entry
  b = new gtk.Entry

  h = new gtk.Entry
  s = new gtk.Entry
  v = new gtk.Entry

  hsv = new gtk.Hsv
  hsv.setColor 0.5, 1, 1

  changed = ->
    currentHsv = hsv.getColor()
    h.setText (Math.round currentHsv[0] * 100) / 100
    s.setText (Math.round currentHsv[1] * 100) / 100
    v.setText (Math.round currentHsv[2] * 100) / 100

    currentRgb = gtk.Hsv.hsvToRgb currentHsv...
    r.setText Math.floor currentRgb[0] * 255
    g.setText Math.floor currentRgb[1] * 255
    b.setText Math.floor currentRgb[2] * 255

  hsv.on 'move', changed
  hsv.on 'changed', changed

  vbox   = new gtk.Vbox
  vbox.add r
  vbox.add g
  vbox.add b

  vbox.add h
  vbox.add s
  vbox.add v

  hbox = new gtk.Hbox
  hbox.add hsv
  hbox.add vbox

  window = new gtk.Window
  window.add hbox

  window.setTitle 'Node'
  window.setResizable true

  window.show()
