// https://gist.github.com/tesch1/1126425eb7cb1dfea35c9f0480111908

// circ_shift.h
// https://stackoverflow.com/questions/46077242/eigen-modifyable-custom-expression/46301503#46301503
// this file implements circShift, fftshift, and ifftshift for Eigen vectors/matrices.
//

#pragma once
#include <Eigen/Core>

template <bool B> using bool_constant = std::integral_constant<bool, B>;

namespace helper
{
namespace detail
{
template <typename T>
constexpr std::true_type is_matrix(Eigen::MatrixBase<T>);
std::false_type constexpr is_matrix(...);

template <typename T>
constexpr std::true_type is_array(Eigen::ArrayBase<T>);
std::false_type constexpr is_array(...);
}


template <typename T>
struct is_matrix : decltype(detail::is_matrix(std::declval<std::remove_cv_t<T>>()))
{
};

template <typename T>
struct is_array : decltype(detail::is_array(std::declval<std::remove_cv_t<T>>()))
{
};

template <typename T>
using is_matrix_or_array = bool_constant<is_array<T>::value || is_matrix<T>::value>;

/*
 * Index something if it's not an scalar
 */
template <typename T, typename std::enable_if<is_matrix_or_array<T>::value, int>::type = 0>
auto index_if_necessary(T&& thing, Eigen::Index idx)
{
  return thing(idx);
}

/*
 * Overload for scalar.
 */
template <typename T, typename std::enable_if<std::is_scalar<std::decay_t<T>>::value, int>::type = 0>
  auto index_if_necessary(T&& thing, Eigen::Index)
{
  return thing;
}
}

namespace Eigen
{
template <typename XprType, typename RowIndices, typename ColIndices>
class CircShiftedView;

namespace internal
{
template <typename XprType, typename RowIndices, typename ColIndices>
struct traits<CircShiftedView<XprType, RowIndices, ColIndices>>
  : traits<XprType>
{
  enum
  {
    RowsAtCompileTime = traits<XprType>::RowsAtCompileTime,
    ColsAtCompileTime = traits<XprType>::ColsAtCompileTime,
    MaxRowsAtCompileTime = (RowsAtCompileTime != Dynamic 
                            ? int(RowsAtCompileTime) 
                            : int(traits<XprType>::MaxRowsAtCompileTime)),
    MaxColsAtCompileTime = (ColsAtCompileTime != Dynamic 
                            ? int(ColsAtCompileTime)
                            : int(traits<XprType>::MaxColsAtCompileTime)),
    XprTypeIsRowMajor = (int(traits<XprType>::Flags) & RowMajorBit) != 0,
    IsRowMajor = ((MaxRowsAtCompileTime == 1 && MaxColsAtCompileTime != 1) ? 1
                  : (MaxColsAtCompileTime == 1 && MaxRowsAtCompileTime != 1) ? 0
                  : XprTypeIsRowMajor),
    FlagsRowMajorBit = IsRowMajor ? RowMajorBit : 0,
    FlagsLvalueBit = is_lvalue<XprType>::value ? LvalueBit : 0,
    Flags = (traits<XprType>::Flags & HereditaryBits) | FlagsLvalueBit | FlagsRowMajorBit
  };
};
}

template <typename XprType, typename RowShift, typename ColShift, typename StorageKind>
class CircShiftedViewImpl;


template <typename XprType, typename RowShift, typename ColShift>
class CircShiftedView : public CircShiftedViewImpl<XprType, RowShift, ColShift, 
                                                   typename internal::traits<XprType>::StorageKind>
{
public:
  typedef typename CircShiftedViewImpl<XprType, RowShift, ColShift,
                                       typename internal::traits<XprType>::StorageKind>::Base Base;
  EIGEN_GENERIC_PUBLIC_INTERFACE(CircShiftedView)
  EIGEN_INHERIT_ASSIGNMENT_OPERATORS(CircShiftedView)

  typedef typename internal::ref_selector<XprType>::non_const_type MatrixTypeNested;
  typedef typename internal::remove_all<XprType>::type NestedExpression;

  template <typename T0, typename T1>
  CircShiftedView(XprType& xpr, const T0& rowShift, const T1& colShift)
    : m_xpr(xpr), m_rowShift(rowShift), m_colShift(colShift)
  {
    for (auto c = 0; c < xpr.cols(); ++c)
      assert(std::abs(helper::index_if_necessary(m_rowShift, c)) < m_xpr.rows()); // row shift must be within +- rows()-1
    for (auto r = 0; r < xpr.rows(); ++r)
      assert(std::abs(helper::index_if_necessary(m_colShift, r)) < m_xpr.cols()); // col shift must be within +- cols()-1
  }

  /** \returns number of rows */
  Index rows() const { return m_xpr.rows(); }

  /** \returns number of columns */
  Index cols() const { return m_xpr.cols(); }

  /** \returns the nested expression */
  const typename internal::remove_all<XprType>::type&
  nestedExpression() const { return m_xpr; }

  /** \returns the nested expression */
  typename internal::remove_reference<XprType>::type&
  nestedExpression() { return m_xpr.const_cast_derived(); }

  EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE
  Index getRowIdx(Index row, Index col) const
  {
    Index R = m_xpr.rows();
    assert(row >= 0 && row < R && col >= 0 && col < m_xpr.cols());
    Index r = row - helper::index_if_necessary(m_rowShift, col);
    if (r >= R)
      return r - R;
    if (r < 0)
      return r + R;
    return r;
  }

  EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE
  Index getColIdx(Index row, Index col) const
  {
    Index C = m_xpr.cols();
    assert(row >= 0 && row < m_xpr.rows() && col >= 0 && col < C);
    Index c = col - helper::index_if_necessary(m_colShift, row);
    if (c >= C)
      return c - C;
    if (c < 0)
      return c + C;
    return c;
  }

protected:
  MatrixTypeNested m_xpr;
  RowShift m_rowShift;
  ColShift m_colShift;
};


// Generic API dispatcher
template <typename XprType, typename RowIndices, typename ColIndices, typename StorageKind>
class CircShiftedViewImpl
  : public internal::generic_xpr_base<CircShiftedView<XprType, RowIndices, ColIndices>>::type
{
 public:
  typedef typename internal::generic_xpr_base<CircShiftedView<XprType, RowIndices, ColIndices>>::type Base;
};

namespace internal
{
template <typename ArgType, typename RowIndices, typename ColIndices>
struct unary_evaluator<CircShiftedView<ArgType, RowIndices, ColIndices>, IndexBased>
  : evaluator_base<CircShiftedView<ArgType, RowIndices, ColIndices>>
{
  typedef CircShiftedView<ArgType, RowIndices, ColIndices> XprType;

  enum
  {
    CoeffReadCost = (evaluator<ArgType>::CoeffReadCost 
                     + NumTraits<Index>::AddCost  /* for comparison */
                     + NumTraits<Index>::AddCost) /* for addition */,
    Flags = (evaluator<ArgType>::Flags & HereditaryBits),
    Alignment = 0
  };

  EIGEN_DEVICE_FUNC explicit unary_evaluator(const XprType& xpr) : m_argImpl(xpr.nestedExpression()), m_xpr(xpr)
  {
    EIGEN_INTERNAL_CHECK_COST_VALUE(CoeffReadCost);
  }

  typedef typename XprType::Scalar Scalar;
  typedef typename XprType::CoeffReturnType CoeffReturnType;


  EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE
    CoeffReturnType coeff(Index row, Index col) const
  {
    return m_argImpl.coeff(m_xpr.getRowIdx(row, col), m_xpr.getColIdx(row, col));
  }

  EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE
    CoeffReturnType coeff(Index idx) const
  {
    if (m_xpr.cols() == 1)
      return m_argImpl.coeff(m_xpr.getRowIdx(idx, 1), 1);
    if (m_xpr.rows() == 1)
      return m_argImpl.coeff(1, m_xpr.getColIdx(1, idx));
    assert(m_xpr.cols() == 1 || m_xpr.rows() == 1);
    // default no-assert case - assume col vector
    return m_argImpl.coeff(m_xpr.getRowIdx(idx, 1), 1);
  }

  EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE
    Scalar& coeffRef(Index row, Index col)
  {
    assert(row >= 0 && row < m_xpr.rows() && col >= 0 && col < m_xpr.cols());

    return m_argImpl.coeffRef(m_xpr.getRowIdx(row, col), m_xpr.getColIdx(row, col));
  }

 protected:

  evaluator<ArgType> m_argImpl;
  const XprType& m_xpr;
};
} // end namespace internal
} // end namespace Eigen

template <typename XprType, typename RowShift, typename ColShift>
auto circShift(Eigen::DenseBase<XprType>& x, RowShift r, ColShift c)
{
    return Eigen::CircShiftedView<XprType, RowShift, ColShift>(x.derived(), r, c);
}

template <typename XprType>
auto fftshift(Eigen::DenseBase<XprType>& x)
{
  Eigen::Index rs = x.rows() / 2;
  Eigen::Index cs = x.cols() / 2;
  return Eigen::CircShiftedView<XprType, Eigen::Index, Eigen::Index>(x.derived(), rs, cs);
}

template <typename XprType>
auto ifftshift(Eigen::DenseBase<XprType>& x)
{
  Eigen::Index rs = (x.rows() + 1) / 2;
  Eigen::Index cs = (x.cols() + 1) / 2;
  return Eigen::CircShiftedView<XprType, Eigen::Index, Eigen::Index>(x.derived(), rs, cs);
}