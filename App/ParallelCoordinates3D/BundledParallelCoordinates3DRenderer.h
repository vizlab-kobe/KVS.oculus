#pragma once
#include "ParallelCoordinates3DRenderer.h"


namespace local
{

class BundledParallelCoordinates3DRenderer : public local::ParallelCoordinates3DRenderer
{
    kvsModule( local::BundledParallelCoordinates3DRenderer, Renderer );
    kvsModuleBaseClass( local::ParallelCoordinates3DRenderer );

private:
    float m_reduced_plane_scale;
    kvs::ValueTable<float> m_reduced_data;
    kvs::ValueArray<float> m_reduced_min_values;
    kvs::ValueArray<float> m_reduced_max_values;
    size_t m_bundled_position;
    float m_bundled_line_size;
    kvs::RGBColor m_bundled_line_color;

public:
    BundledParallelCoordinates3DRenderer();

    void setReducedPlaneScale( const float scale ) { m_reduced_plane_scale = scale; }
    void setReducedData( const kvs::ValueTable<float>& data )
    {
        m_reduced_data = data;
        m_reduced_min_values.allocate( data.columnSize() );
        m_reduced_max_values.allocate( data.columnSize() );
        for ( size_t i = 0; i < data.columnSize(); i++ )
        {
            auto result = std::minmax_element( m_reduced_data[i].begin(), m_reduced_data[i].end() );
            m_reduced_min_values[i] = *result.first;
            m_reduced_max_values[i] = *result.second;
        }
    }
    void setBundledPosition( const size_t p ) { m_bundled_position = p; }
    void setBundledLineSize( const float size ) { m_bundled_line_size = size; }
    void setBundledLineColor( const kvs::RGBColor& color ) { m_bundled_line_color = color; }

    float reducedPlaneScale() const { return m_reduced_plane_scale; }
    const kvs::ValueTable<float>& reducedData() const { return m_reduced_data; }
    size_t bundledPosition() const { return m_bundled_position; }
    float bundledLineSize() const { return m_bundled_line_size; }
    const kvs::RGBColor& bundledLineColor() const { return m_bundled_line_color; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void draw_bundled_lines( const kvs::TableObject* table, const float dpr = 1.0f );
};

} // end of namespace local
