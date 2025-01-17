#include <lib/gui/eslider.h>

int eSlider::defaultSliderBorderWidth = eSlider::DefaultBorderWidth;

eSlider::eSlider(eWidget *parent)
	:eWidget(parent), m_have_border_color(false), m_have_foreground_color(false), m_have_background_color(false), m_scrollbar(false),
	m_min(0), m_max(0), m_value(0), m_start(0), m_orientation(orHorizontal), m_orientation_swapped(0),
	m_border_width(0)
{
	m_border_width = eSlider::defaultSliderBorderWidth;
}

void eSlider::setIsScrollbar()
{
	m_scrollbar = true;
}

void eSlider::setPixmap(ePtr<gPixmap> &pixmap)
{
	setPixmap(pixmap.operator->());
}

void eSlider::setPixmap(gPixmap *pixmap)
{
	m_pixmap = pixmap;
	event(evtChangedSlider);
}

void eSlider::setBackgroundPixmap(ePtr<gPixmap> &pixmap)
{
	setBackgroundPixmap(pixmap.operator->());
}

void eSlider::setBackgroundPixmap(gPixmap *pixmap)
{
	m_backgroundpixmap = pixmap;
	invalidate();
}

void eSlider::setBorderWidth(int width)
{
	m_border_width=width;
	invalidate();
}

void eSlider::setBorderColor(const gRGB &color)
{
	m_border_color=color;
	m_have_border_color=true;
	invalidate();
}

void eSlider::setForegroundColor(const gRGB &color)
{
	m_foreground_color = color;
	m_have_foreground_color = true;
	invalidate();
}

void eSlider::setBackgroundColor(const gRGB &color)
{
	m_background_color = color;
	m_have_background_color = true;
	invalidate();
}

void eSlider::setAlphatest(int alphatest)
{
	m_alphatest = alphatest;
	setTransparent(alphatest);
}

int eSlider::event(int event, void *data, void *data2)
{
	switch (event)
	{
	case evtPaint:
	{
		ePtr<eWindowStyle> style;

		eSize s(size());
		getStyle(style);
		/* paint background */
		eWidget::event(evtPaint, data, data2);

		gPainter &painter = *(gPainter*)data2;

		if (m_backgroundpixmap)
		{
			painter.blit(m_backgroundpixmap, ePoint(0, 0), eRect(), isTransparent() ? gPainter::BT_ALPHATEST : 0);
		}
		else if(m_have_background_color) {
			painter.setBackgroundColor(m_background_color);
			painter.clear();
		}

		style->setStyle(painter, m_scrollbar ? eWindowStyle::styleScollbar : eWindowStyle::styleSlider );

		if (!m_pixmap)
		{
			if (m_have_foreground_color)
				painter.setForegroundColor(m_foreground_color);
			painter.fill(m_currently_filled);
		}
		else
			painter.blit(m_pixmap, ePoint(0, 0), m_currently_filled.extends, isTransparent() ? gPainter::BT_ALPHATEST : 0);

		// Border
		if(m_border_width>0) {

			if (m_have_border_color)
				painter.setForegroundColor(m_border_color);
			else {
				style->setStyle(painter, m_scrollbar ? eWindowStyle::styleScollbarBorder : eWindowStyle::styleSliderBorder );
			}

			painter.fill(eRect(0, 0, s.width(), m_border_width));
			painter.fill(eRect(0, m_border_width, m_border_width, s.height() - m_border_width));
			painter.fill(eRect(m_border_width, s.height() - m_border_width, s.width() - m_border_width, m_border_width));
			painter.fill(eRect(s.width() - m_border_width, m_border_width, m_border_width, s.height() - m_border_width));
		}

		return 0;
	}
	case evtChangedSlider:
	{
		int num_pix = 0, start_pix = 0;
		gRegion old_currently_filled = m_currently_filled;

		// calculate the pixel size of the thumb
		int offset = m_border_width * 2;
		int pixsize = (m_orientation == orHorizontal) ? size().width()-offset : size().height()-offset;

		if (m_min < m_max)
		{

			int val_range = m_max - m_min;
			// calculate the start_pix and num_pix with correct scaling and repective borderwidth
			start_pix = (m_start * pixsize / val_range) + m_border_width;
			num_pix = (m_value * pixsize / val_range) + m_border_width - start_pix;

			if (m_orientation_swapped)
				start_pix = pixsize - num_pix - start_pix;

		}

		if  (start_pix < 0)
		{
			num_pix += start_pix;
			start_pix = 0;
		}

		if (num_pix < 0)
			num_pix = 0;

		if (m_orientation == orHorizontal)
			m_currently_filled = eRect(start_pix, m_border_width, num_pix, pixsize);
		else
			m_currently_filled = eRect(m_border_width, start_pix, pixsize, num_pix);

		// redraw what *was* filled before and now isn't.
		invalidate(m_currently_filled - old_currently_filled);
		// redraw what wasn't filled before and is now.
		invalidate(old_currently_filled - m_currently_filled);

		return 0;
	}
	default:
		return eWidget::event(event, data, data2);
	}
}

void eSlider::setValue(int value)
{
	m_value = value;
	event(evtChangedSlider);
}

void eSlider::setStartEnd(int start, int end)
{
	m_value = end;
	m_start = start;
	event(evtChangedSlider);
}

void eSlider::setOrientation(int orientation, int swapped)
{
	m_orientation = orientation;
	m_orientation_swapped = swapped;
	event(evtChangedSlider);
}

void eSlider::setRange(int min, int max)
{
	m_min = min;
	m_max = max;
	event(evtChangedSlider);
}

// Mapping Functions
void eSlider::setScrollbarForegroundPixmap(ePtr<gPixmap> &pixmap)
{
	setPixmap(pixmap.operator->());
}

void eSlider::setScrollbarForegroundPixmap(gPixmap *pixmap)
{
	setPixmap(pixmap);
}

void eSlider::setScrollbarBackgroundPixmap(ePtr<gPixmap> &pixmap)
{
	setBackgroundPixmap(pixmap.operator->());
}

void eSlider::setScrollbarBackgroundPixmap(gPixmap *pixmap)
{
	setBackgroundPixmap(pixmap);
}

void eSlider::setScrollbarBorderWidth(int width)
{
	setBorderWidth(width);
}

void eSlider::setScrollbarBorderColor(const gRGB &color)
{
	setBorderColor(color);
}

void eSlider::setScrollbarForegroundColor(const gRGB &color)
{
	setForegroundColor(color);
}

void eSlider::setScrollbarBackgroundColor(const gRGB &color)
{
	setBackgroundColor(color);
}
