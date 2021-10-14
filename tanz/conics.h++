#include <Eigen/Dense>

namespace tz {

using conic_params_t = std::array< double, 6 >;

conic_params_t
fit_conic_params( std::vector< Eigen::Vector2f > const & obs );

conic_params_t
fit_conic_params( std::vector< Eigen::Vector2d > const & obs );
/* Computes the ellipsis paramters (A, B, C, D, E, F) for
   the matrix representation:

   Q = \begin{matrix}
   A & B & D \\
   B & C & E \\
   D & E & F \\
   \end{matrix}

   It does so by minimising the algebraic least squares error.
   While easy to compute, it is not statistically meaningful.

   The parameters are scaled so that Q will have det(Q) = -1.
   If no conic can be fitted, an exception of type std::domain_error
   will be thrown.
*/

Eigen::Matrix3d
conic_matrix( conic_params_t const & );

conic_params_t
conic_params( Eigen::Matrix3d const & );
/* Symmetry assumed. */

Eigen::Affine2d
conic_canonical_transform( conic_params_t const & );

conic_params_t
conic_canonical_params( conic_params_t const & );
/* Prerequisite: conic_is_lines( p ) == false,
   with the "right" choice of eps ... */

conic_params_t
conic_canonical_params_non_parabola( conic_params_t const & p_ );

conic_params_t
conic_canonical_params_parabola( conic_params_t const & p_ );

Eigen::Vector2d
conic_center( conic_params_t const & p );
/* Returns the center of the conic.  The center is only defined
   if the conic is a parabola, an ellipsis or two lines.

   See
   https://en.wikipedia.org/wiki/Matrix_representation_of_conic_sections#Center
*/

bool
conic_is_parabola( conic_params_t const &, double eps = 32 * std::numeric_limits< double >::epsilon());

bool
conic_is_hyperbola( conic_params_t const &, double eps = 32 * std::numeric_limits< double >::epsilon());

bool
conic_is_ellipsis( conic_params_t const & );

bool
conic_is_empty( conic_params_t const & ); // imaginary ellipsis

bool
conic_is_lines( conic_params_t const &, double eps = 32 * std::numeric_limits< double >::epsilon());
}
